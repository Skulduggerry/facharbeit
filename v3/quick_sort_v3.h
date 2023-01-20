#ifndef CODE_QUICK_SORT_V3_H
#define CODE_QUICK_SORT_V3_H

#include "../definitions.h"

namespace sort {
    template<std::random_access_iterator It>
    inline std::pair<It, It> three_way_partition(It begin, It end) {
        It left_ptr = begin;
        It equal_ptr_left = left_ptr;
        It right_ptr = end - 1;
        It equal_ptr_right = right_ptr;
        const auto pivot = *right_ptr;

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
    inline void three_way_quicksort(It begin, It end) {
        if (std::distance(begin, end) <= 1)
            return;

        auto [begin_same, end_same] = three_way_partition(begin, end);
        three_way_quicksort(begin, begin_same);
        three_way_quicksort(end_same, end);
    }
}

#endif //CODE_QUICK_SORT_V3_H
