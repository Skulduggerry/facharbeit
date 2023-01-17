#ifndef CODE_QUICK_SORT_V2_H
#define CODE_QUICK_SORT_V2_H

#include "../definitions.h"
#include "../v1/quick_sort_v1.h"
#include <cmath>

namespace sort {
    namespace partition {
        template<std::random_access_iterator It>
        inline It random_partition(It begin, It end) {
            auto random_pos = rand() % (std::distance(begin, end) + 1);
            std::iter_swap(std::next(begin, random_pos), std::prev(end));
            return default_partition(begin, end);
        }
    }

    template<std::random_access_iterator It>
    inline void quicksort_random_partition(It begin, It end) {
        if (std::distance(begin, end) < 2) return;
        It pivot = partition::random_partition(begin, end);
        quicksort(begin, pivot);
        quicksort(std::next(pivot), end);
    }
}


#endif //CODE_QUICK_SORT_V2_H