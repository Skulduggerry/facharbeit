#ifndef CODE_INSERTION_SORT_H
#define CODE_INSERTION_SORT_H

#include <numeric>
#include "definitions.h"
#include "utils.h"

namespace sort {
    template<std::bidirectional_iterator It>
    void insertion_sort(It begin, It end) {
        It unsorted_begin = std::next(begin);

        while (unsorted_begin != end) {
            It current = unsorted_begin;
            while (current != begin && *current < *std::prev(current)) {
                std::iter_swap(current, std::prev(current));
                --current;
            }
            ++unsorted_begin;
        }
    }

    SortableGenerator insertion_sort_best_case_generator = [](size_t n, Value) {
        auto sortable_ptr = std::make_unique<Sortable>(n);
        std::iota(sortable_ptr->begin(), sortable_ptr->end(), 0);
        return sortable_ptr;
    };

    SortableGenerator insertion_sort_worst_case_generator = [](size_t n, Value) {
        auto sortable_ptr = std::make_unique<Sortable>(n);
        std::iota(sortable_ptr->rbegin(), sortable_ptr->rend(), 0);
        return sortable_ptr;
    };
}

#endif //CODE_INSERTION_SORT_H
