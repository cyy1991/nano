#include "task_wine.h"
#include "text/to_params.h"
#include "text/from_params.h"

namespace nano
{
        wine_task_t::wine_task_t(const string_t& config) :
                mem_csv_task_t(dim3d_t{13, 1, 1}, dim3d_t{3, 1, 1}, 1,
                to_params(config, "dir", string_t(std::getenv("HOME")) + "/experiments/databases/wine"))
        {
        }

        bool wine_task_t::populate()
        {
                const auto path = from_params<string_t>(config(), "dir") + "/wine.data";
                const auto task_name = "WINE";

                const auto expected_samples = size_t(178);

                const auto scale1 = scalar_t(1e-1);
                const auto scale3 = scalar_t(1e-3);

                const auto scales = scalars_t{scale1, 1, 1, scale1, scale1, 1, 1, 1, 1, 1, 1, 1, scale3};
                const auto labels = strings_t{"1", "2", "3"};
                const auto label_column = size_t(0);

                return mem_csv_task_t::load_classification(path, task_name, expected_samples, scales, labels, label_column);
        }
}
