#include "math/random.h"
#include "iterator_noise.h"
#include "tensor/numeric.h"

using namespace nano;

iterator_noise_t::iterator_noise_t(const string_t& config) :
        iterator_t(to_params(config, "noise", "0.1[0,1]"))
{
}

sample_t iterator_noise_t::get(const task_t& task, const fold_t& fold, const size_t index) const
{
        sample_t sample = task.get(fold, index);

        const auto noise = from_params<scalar_t>(config(), "noise");

        // keep input and add some salt & pepper noise
        auto rng = make_rng<scalar_t>(-1, +1);
        nano::add_random([&] () { return rng() * noise; }, sample.m_input);

        return sample;
}
