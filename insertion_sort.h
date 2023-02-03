#ifndef CODE_INSERTION_SORT_H
#define CODE_INSERTION_SORT_H

#include <numeric>
#include "definitions.h"
#include "utils.h"

namespace sort {
    template<std::bidirectional_iterator It>
    inline void insertion_sort(It begin, It end) {
        It unsorted_begin = std::next(begin);

        while (unsorted_begin < end) {
            It current = unsorted_begin;
            while (current != begin && *current < *std::prev(current)) {
                std::iter_swap(current, std::prev(current));
                --current;
            }
            ++unsorted_begin;
        }
    }
}

#endif //CODE_INSERTION_SORT_H
