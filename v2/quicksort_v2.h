#ifndef CODE_QUICKSORT_V2_H
#define CODE_QUICKSORT_V2_H

#include "../definitions.h"
#include "../v1/quicksort_v1.h"

namespace sort {
    namespace partition {
        template<std::random_access_iterator It>
        inline It partition_median_of_3(It begin, It end) {
            auto first_elem = *begin;
            It middle = std::next(begin, (end - begin) / 2 - 1);
            auto middle_elem = *middle;
            It last = std::prev(end);
            auto last_elem = *last;

            if ((first_elem > middle_elem) ^ (first_elem > last_elem))
                std::iter_swap(begin, last);
            else if ((middle_elem < first_elem) ^ (middle_elem < last_elem))
                std::iter_swap(middle, last);

            return default_partition(begin, end);
        }
    }

    template<std::random_access_iterator It>
    inline void quick_sort_median_of_3(It begin, It end) {
        if (std::distance(begin, end) < 2) return;
        It pivot = partition::partition_median_of_3(begin, end);
        quicksort(begin, pivot);
        quicksort(std::next(pivot), end);
    }
}


#endif //CODE_QUICKSORT_V2_H