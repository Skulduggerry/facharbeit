#ifndef CODE_INSERTION_SORT_V1_H
#define CODE_INSERTION_SORT_V1_H

#include "../definitions.h"
#include <numeric>

namespace sort {
    template<std::bidirectional_iterator It>
    void insertion_sort(It begin, It end) {
        It unsorted_begin = std::next(begin);

        while (unsorted_begin != end) {
            It current = unsorted_begin;
            while (current != begin && *current < *std::prev(current)) {
                std::iter_swap(current, std::prev(current));
                std::advance(current, -1);
            }
            std::advance(unsorted_begin, 1);
        }
    }

    SortableGenerator insertion_sort_best_case_generator = [](size_t n) {
        Sortable sortable(n);
        std::iota(sortable.begin(), sortable.end(), 0);
        return sortable;
    };

    SortableGenerator insertion_sort_worst_case_generator = [](size_t n) {
        Sortable sortable(n);
        std::iota(sortable.rbegin(), sortable.rend(), 0);
        return sortable;
    };
}

#endif //CODE_INSERTION_SORT_V1_H