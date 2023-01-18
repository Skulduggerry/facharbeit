#ifndef CODE_QUICK_SORT_V4_H
#define CODE_QUICK_SORT_V4_H

#include "../definitions.h"
#include "../v3/quick_sort_v3.h"
#include <cmath>

namespace sort {
    template<std::random_access_iterator It>
    inline void tail_quick_sort(It begin, It end) {
        while (std::distance(begin, end) > 1) {
            It pivot = partition::default_partition(begin, end);
            tail_quick_sort(begin, pivot);
            begin = std::next(pivot);
        }
    }
}

#endif //CODE_QUICK_SORT_V4_H
