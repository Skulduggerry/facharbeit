#ifndef CODE_QUICKSORT_H
#define CODE_QUICKSORT_H

#include "definitions.h"
#include "utils.h"
#include <numeric>

namespace sort {
    template<std::random_access_iterator It>
    inline It default_partition(It begin, It end) {
        It ptr_left = begin;
        It ptr_right = std::prev(end, 2);
        It pivot_ptr = std::prev(end);
        auto pivot = *pivot_ptr;

        while (0 < std::distance(ptr_left, ptr_right)) {
            while (0 < std::distance(ptr_left, ptr_right) && *ptr_left <= pivot) {
                ++ptr_left;
            }

            while (0 < std::distance(ptr_left, ptr_right) && pivot <= *ptr_right) {
                --ptr_right;
            }

            std::iter_swap(ptr_left, ptr_right);
        }

        while (std::distance(ptr_left, end) && *ptr_left < pivot) {
            ++ptr_left;
        }
        std::iter_swap(ptr_left, pivot_ptr);
        return ptr_left;
    }

    template<std::random_access_iterator It>
    inline It partition_median_of_3(It begin, It end) {
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

        return default_partition(begin, end);
    }

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
    inline void quicksort(It begin, It end) {
        if (std::distance(begin, end) < 2) return;
        It pivot = default_partition(begin, end);
        quicksort(begin, pivot);
        quicksort(std::next(pivot), end);
    }

    template<std::random_access_iterator It>
    inline void quick_sort_median_of_3(It begin, It end) {
        if (std::distance(begin, end) < 2) return;
        It pivot = partition_median_of_3(begin, end);
        quick_sort_median_of_3(begin, pivot);
        quick_sort_median_of_3(std::next(pivot), end);
    }

    template<std::random_access_iterator It>
    inline void three_way_quicksort(It begin, It end) {
        if (std::distance(begin, end) <= 1) return;
        auto [begin_same, end_same] = three_way_partition(begin, end);
        three_way_quicksort(begin, begin_same);
        three_way_quicksort(end_same, end);
    }


    SortableGenerator quicksort_worst_case_generator = [](size_t n, Value) {
        auto sortable_ptr = std::make_unique<Sortable>(n);
        std::iota(sortable_ptr->begin(), sortable_ptr->end(), 0);
        return sortable_ptr;
    };

    SortableGenerator same_number_generator = [](size_t n, Value) {
        return std::make_unique<Sortable>(n, 42);
    };

    template<std::random_access_iterator It>
    void make_best_case(It begin, It end) {
        if (std::distance(begin, end) < 2) return;
        It middle = std::next(begin, std::distance(begin, end) / 2);
        make_best_case(begin, middle);
        make_best_case(std::next(middle), end);
        std::iter_swap(middle, std::prev(end));
    }

    SortableGenerator quicksort_best_case_generator = [](size_t n, Value) {
        auto sortable_ptr = std::make_unique<Sortable>(n);
        std::iota(sortable_ptr->begin(), sortable_ptr->end(), 0);
        make_best_case(sortable_ptr->begin(), sortable_ptr->end());
        return sortable_ptr;
    };
}

#endif //CODE_QUICKSORT_H
