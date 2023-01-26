#ifndef CODE_INTRO_SORT_H
#define CODE_INTRO_SORT_H

#include "definitions.h"
#include "quicksort.h"
#include "insertion_sort.h"

namespace sort {
    template<std::random_access_iterator It>
    inline std::pair<It, It> _intro_sort_partition_(It begin, It end) {
        {
            auto first_elem = *begin;
            It middle = std::next(begin, (end - begin) / 2 - 1);
            auto middle_elem = *middle;
            It last = std::prev(end);
            auto last_elem = *last;

            if ((first_elem > middle_elem) ^ (first_elem > last_elem))
                std::iter_swap(begin, last);
            else if ((middle_elem < first_elem) ^ (middle_elem < last_elem))
                std::iter_swap(middle, last);
        }

        return three_way_partition(begin, end);
    }

    template<std::random_access_iterator It>
    inline void intro_sort(It begin, It end) {
        if(std::distance(begin, end) < 2) return;

        if (std::distance(begin, end) < TO_PRIMITIVE_THRESHOLD) {
            insertion_sort(begin, end);
            return;
        }

        auto [begin_same, end_same] = _intro_sort_partition_(begin, end);
        intro_sort(begin, begin_same);
        intro_sort(end_same, end);
    }
}

#endif //CODE_INTRO_SORT_H