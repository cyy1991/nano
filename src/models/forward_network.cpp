#include "timer.h"
#include "logger.h"
#include "text/align.h"
#include "math/numeric.h"
#include "text/to_string.h"
#include "text/algorithm.h"
#include "forward_network.h"
#include "tensor/serialize.h"
#include <iomanip>

namespace nano
{
        forward_network_t::layer_info_t::layer_info_t(const string_t& name, rlayer_t layer) :
                m_name(name), m_layer(std::move(layer))
        {
        }

        forward_network_t::layer_info_t::layer_info_t(const layer_info_t& other) :
                m_name(other.m_name),
                m_layer(other.m_layer->clone()),
                m_output_timings(other.m_output_timings),
                m_ginput_timings(other.m_ginput_timings),
                m_gparam_timings(other.m_gparam_timings)
        {
        }

        const tensor3d_t& forward_network_t::layer_info_t::output(const tensor3d_t& input)
        {
                const timer_t timer;
                const auto& ret = m_layer->output(input);
                m_output_timings(static_cast<size_t>(timer.microseconds().count()));
                return ret;
        }

        const tensor3d_t& forward_network_t::layer_info_t::ginput(const tensor3d_t& output)
        {
                const timer_t timer;
                const auto& ret = m_layer->ginput(output);
                m_ginput_timings(static_cast<size_t>(timer.microseconds().count()));
                return ret;
        }

        scalar_t* forward_network_t::layer_info_t::gparam(const tensor3d_t& output, scalar_t* gparam)
        {
                const timer_t timer;
                gparam -= m_layer->psize();
                m_layer->gparam(output, gparam);
                m_gparam_timings(static_cast<size_t>(timer.microseconds().count()));
                return gparam;
        }

        forward_network_t::forward_network_t(const string_t& parameters) :
                model_t(parameters)
        {
        }

        rmodel_t forward_network_t::clone() const
        {
                return std::make_unique<forward_network_t>(*this);
        }

        const tensor3d_t& forward_network_t::output(const tensor3d_t& _input)
        {
                const tensor3d_t* input = &_input;
                for (size_t l = 0; l < n_layers(); ++ l)
                {
                        input = &m_layers[l].output(*input);
                }

                return *input;
        }

        const tensor3d_t& forward_network_t::ginput(const vector_t& _output)
        {
                assert(_output.size() == odims().size());
                assert(!m_layers.empty());

                m_odata = tensor::map_tensor(_output.data(), odims());

                return ginput(m_odata);
        }

        const tensor3d_t& forward_network_t::ginput(const tensor3d_t& _output)
        {
                const tensor3d_t* output = &_output;
                for (size_t l = n_layers(); l > 0; l --)
                {
                        output = &m_layers[l - 1].ginput(*output);
                }

                return *output;
        }

        const vector_t& forward_network_t::gparam(const vector_t& _output)
        {
                assert(_output.size() == odims().size());
                assert(!m_layers.empty());

                m_odata = tensor::map_tensor(_output.data(), odims());

                return gparam(m_odata);
        }

        const vector_t& forward_network_t::gparam(const tensor3d_t& _output)
        {
                m_gparam.resize(psize());

                // backward step
                const tensor3d_t* output = &_output;
                scalar_t* gparam = m_gparam.data() + m_gparam.size();
                for (size_t l = n_layers(); l > 0; l --)
                {
                        gparam = m_layers[l - 1].gparam(*output, gparam);
                        if (l > 1)
                        {
                                output = &m_layers[l - 1].ginput(*output);
                        }
                }

                return m_gparam;
        }

        bool forward_network_t::save_params(vector_t& x) const
        {
                x.resize(psize());

                scalar_t* px = x.data();
                for (const auto& layer : m_layers)
                {
                        px = layer.m_layer->save_params(px);
                }

                return true;
        }

        bool forward_network_t::load_params(const vector_t& x)
        {
                if (x.size() == psize())
                {
                        const scalar_t* px = x.data();
                        for (const auto& layer : m_layers)
                        {
                                px = layer.m_layer->load_params(px);
                        }

                        return true;
                }
                else
                {
                        return false;
                }
        }

        void forward_network_t::zero_params()
        {
                for (const auto& layer : m_layers)
                {
                        layer.m_layer->zero_params();
                }
        }

        void forward_network_t::random_params()
        {
                for (const auto& layer : m_layers)
                {
                        const auto fanin = layer.m_layer->idims() * layer.m_layer->irows() * layer.m_layer->icols();
                        const auto fanout = layer.m_layer->odims() * layer.m_layer->orows() * layer.m_layer->ocols();

                        const auto div = static_cast<scalar_t>(fanin + fanout);
                        const auto min = -std::sqrt(6 / (1 + div));
                        const auto max = +std::sqrt(6 / (1 + div));

                        layer.m_layer->random_params(min, max);
                }
        }

        tensor_size_t forward_network_t::resize(bool verbose)
        {
                tensor3d_t input(idims());
                tensor_size_t n_params = 0;

                m_layers.clear();

                strings_t layer_ids;

                // create layers
                const string_t config = this->config();

                const strings_t net_params = nano::split(config, ";");
                for (size_t l = 0; l < net_params.size(); ++ l)
                {
                        if (net_params[l].size() <= 1)
                        {
                                continue;
                        }

                        const strings_t layer_tokens = nano::split(net_params[l], ":");
                        if (layer_tokens.size() != 2 && layer_tokens.size() != 1)
                        {
                                log_error() << "forward network: invalid layer description <"
                                            << net_params[l] << ">! expecting <layer_id[:layer_parameters]>!";
                                throw std::invalid_argument("invalid layer description");
                        }

                        const string_t layer_id = layer_tokens[0];
                        const string_t layer_params = layer_tokens.size() == 2 ? layer_tokens[1] : string_t();

                        auto layer = nano::get_layers().get(layer_id, layer_params);
                        n_params += layer->resize(input);

                        const string_t layer_name =
                                "[" + align(to_string(l + 1), 2, alignment::right, '0') + ":" +
                                align(layer_id, 10, alignment::left, '.') + "]";
                        input.resize(layer->odims(), layer->orows(), layer->ocols());

                        m_layers.emplace_back(layer_name, std::move(layer));
                }

                // check output size to match the target
                if (input.dims() != odims())
                {
                        log_error() << "forward network: miss-matching output size! expecting " << odims() << "!";
                        throw std::invalid_argument("invalid output layer description");
                }

                if (verbose)
                {
                        print();
                }

                return n_params;
        }

        void forward_network_t::print() const
        {
                for (size_t l = 0; l < n_layers(); ++ l)
                {
                        const auto& name = m_layers[l].m_name;
                        const auto& layer = m_layers[l].m_layer;

                        log_info()
                                << "forward network " << name
                                << ": in(" << layer->idims() << "x" << layer->irows() << "x" << layer->icols() << ") -> "
                                << "out(" << layer->odims() << "x" << layer->orows() << "x" << layer->ocols()
                                << "), parameters = " << layer->psize() << ", FLOPs = " << layer->flops() << ".";
                }
        }

        tensor_size_t forward_network_t::psize() const
        {
                tensor_size_t nparams = 0;
                for (const auto& layer : m_layers)
                {
                        nparams += layer.m_layer->psize();
                }

                return nparams;
        }

        model_t::timings_t forward_network_t::timings() const
        {
                model_t::timings_t ret;
                for (const auto& layer : m_layers)
                {
                        if (layer.m_output_timings.count() > 1) ret[layer.m_name + " (output)"] = layer.m_output_timings;
                        if (layer.m_ginput_timings.count() > 1) ret[layer.m_name + " (ginput)"] = layer.m_ginput_timings;
                        if (layer.m_gparam_timings.count() > 1) ret[layer.m_name + " (gparam)"] = layer.m_gparam_timings;
                }

                return ret;
        }
}
