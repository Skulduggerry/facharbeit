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
    inline void _intro_sort(It begin, It end, uint64_t max_depth) {
        if (std::distance(begin, end) < 2) return;

        if (std::distance(begin, end) < TO_PRIMITIVE_THRESHOLD) {
            insertion_sort(begin, end);
            return;
        }

        if (max_depth == 0) {
            heapsort(begin, end);
            return;
        }

        auto [begin_same, end_same] = _intro_sort_partition_(begin, end);
        _intro_sort(begin, begin_same, --max_depth);
        _intro_sort(end_same, end, max_depth);
    }

    template<std::random_access_iterator It>
    inline void intro_sort(It begin, It end) {
        size_t max_depth = std::floor(log2(std::distance(begin, end) * 2));
        _intro_sort(begin, end, max_depth);
    }
}

#endif //CODE_INTRO_SORT_H