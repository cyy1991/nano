#include "loss.h"
#include <cassert>

namespace ncv
{
        vector_t class_target(size_t ilabel, size_t n_labels)
        {
                assert(ilabel < n_labels);
                
                vector_t target(n_labels);
                target.setConstant(neg_target());
                if (ilabel < n_labels)
                {
                        target[ilabel] = pos_target();
                }
                return target;
        }
}
	
