#include "text/table.h"
#include "text/cmdline.h"
#include "cortex/cortex.h"
#include "thread/thread.h"
#include "thread/loopi.hpp"
#include "cortex/sampler.h"
#include "cortex/util/measure.hpp"
#include "cortex/tasks/task_charset.h"
#include "cortex/util/measure_and_log.hpp"

int main(int argc, char *argv[])
{
        zob::init();

        using namespace zob;

        // parse the command line
        zob::cmdline_t cmdline("benchmark sampling");
        cmdline.add("s", "samples",     "number of samples to use [256, 100000]", "8000");

        cmdline.process(argc, argv);

        // check arguments and options
        const auto cmd_samples = zob::clamp(cmdline.get<size_t>("samples"), 256, 100 * 1000);

        const size_t cmd_rows = 28;
        const size_t cmd_cols = 28;
        const color_mode cmd_color = color_mode::luma;

        const size_t cmd_min_samples = cmd_samples / 8;
        const size_t cmd_max_samples = cmd_min_samples * 8;

        const size_t cmd_min_nthreads = 1;
        const size_t cmd_max_nthreads = zob::n_threads();

        // create synthetic task
        charset_task_t task(charset::numeric, cmd_rows, cmd_cols, cmd_color, cmd_samples);
        task.load("");

        tensors_t inputs(cmd_max_samples);
        vectors_t targets(cmd_max_samples);

        // construct tables to compare sampling
        zob::table_t table("sampling\\threads");

        for (size_t nthreads = cmd_min_nthreads; nthreads <= cmd_max_nthreads; ++ nthreads)
        {
                table.header() << (zob::to_string(nthreads) + "xCPU [us]");
        }

        // evaluate sampling
        for (size_t is = cmd_min_samples; is <= cmd_max_samples; is *= 2)
        {
                const string_t cmd_name = "sample size " + zob::to_string(is);

                zob::table_row_t& row = table.append(cmd_name);

                log_info() << "<<< running test [" << cmd_name << "] ...";

                // select random samples
                sampler_t sampler(task.samples());
                sampler.push(annotation::annotated);
                sampler.push(is);

                const samples_t samples = sampler.get();

                // process the samples
                for (size_t nthreads = cmd_min_nthreads; nthreads <= cmd_max_nthreads; ++ nthreads)
                {
                        zob::pool_t pool(nthreads);

                        const auto micros = zob::measure_robustly_usec([&]
                        {
                                zob::loopi(samples.size(), pool, [&] (size_t i)
                                {
                                        const sample_t& sample = samples[i];
                                        const image_t& image = task.image(sample.m_index);

                                        inputs[i] = image.to_tensor(sample.m_region);
                                        targets[i] = sample.m_target;
                                });
                        }, 1);

                        log_info() << "<<< processed [" << samples.size() << "] samples in " << micros.count() << " us.";

                        row << micros.count();
                }

                log_info();
        }

        // print results
        table.print(std::cout);

        // OK
        log_info() << done;
        return EXIT_SUCCESS;
}
