#ifndef CODE_BUBBLE_SORT_H
#define CODE_BUBBLE_SORT_H

#include "definitions.h"
#include <numeric>

namespace sort {
    template<std::random_access_iterator It>
    void bubble_sort(It begin, It end) {
        if (std::distance(begin, end) < 2) return;
        bool sorted;

        do {
            It current = std::next(begin);
            sorted = true;
            It prev = begin;

            while (current != end) {
                if (*current < *prev) {
                    sorted = false;
                    std::iter_swap(current, prev);
                }

                prev = current;
                std::advance(current, 1);
            }
        } while (!sorted);
    }

    SortableGenerator bubble_sort_best_case_generator = [](size_t n) {
        Sortable sortable(n);
        std::iota(sortable.begin(), sortable.end(), 0);
        return sortable;
    };

    SortableGenerator bubble_sort_worst_case_generator = [](size_t n) {
        Sortable sortable(n);
        std::iota(sortable.rbegin(), sortable.rend(), 0);
        return sortable;
    };
}

#endif //CODE_BUBBLE_SORT_H