#ifndef CODE_COMBINED_SORT_H
#define CODE_COMBINED_SORT_H

#include "definitions.h"
#include "heapsort.h"
#include "insertion_sort.h"
#include <cmath>

namespace sort {

    template<std::random_access_iterator It>
    inline std::pair<It, It> partition(It begin, It end) {
        using ValueType = std::iterator_traits<It>::value_type;
        ValueType pivot;

        It middle = std::next(begin, (end - begin) / 2 - 1);
        It last = std::prev(end);

        {
            if ((*begin > *middle) ^ (*begin > *last)) {
                pivot = *begin;
                std::iter_swap(begin, last);
            } else if ((*middle < *begin) ^ (*middle < *end)) {
                pivot = *middle;
                std::iter_swap(middle, last);
            } else {
                pivot = *last;
            }
        }

        It left_ptr = begin;
        It equal_ptr_left = left_ptr;
        It right_ptr = end - 1;
        It equal_ptr_right = right_ptr;

        while (true) {
            while (*left_ptr < pivot) {
                ++left_ptr;
            }

            while (pivot < *right_ptr) {
                if (left_ptr >= right_ptr) break;
                --right_ptr;
            }

            if (left_ptr >= right_ptr)break;

            std::iter_swap(left_ptr, right_ptr);

            if (pivot == *left_ptr) {
                std::iter_swap(left_ptr, equal_ptr_left);
                ++equal_ptr_left;
            }

            if (pivot == *right_ptr) {
                std::iter_swap(right_ptr, equal_ptr_right);
                --equal_ptr_right;
            }

            ++left_ptr;
            --right_ptr;
        }

        It i = left_ptr - 1;
        for (It k = begin; k < equal_ptr_left; ++k, --i) {
            std::iter_swap(k, i);
        }

        It j = left_ptr;
        for (It k = end - 1; equal_ptr_right < k; --k, ++j) {
            std::iter_swap(j, k);
        }

        return std::make_pair(i + 1, j);
    }

    template<std::random_access_iterator It>
    inline void __intro_sort(It begin, It end, size_t depth) {
        if (std::distance(begin, end) < 17) {
            insertion_sort(begin, end);
            return;
        }

        if (depth == 0) {
            heapsort(begin, end);
            return;
        }

        auto [begin_equal, end_equal] = partition(begin, end);

        __intro_sort(begin, begin_equal, --depth);
        __intro_sort(end_equal, end, depth);
    }

    template<std::random_access_iterator It>
    inline void intro_sort(It begin, It end) {
        size_t max_depth = log2(std::distance(begin, end)) * 2;
        __intro_sort(begin, end, max_depth);
    }
}

#endif //CODE_COMBINED_SORT_H