#ifndef CODE_QUICK_SORT_V1_H
#define CODE_QUICK_SORT_V1_H

#include "../definitions.h"
#include <numeric>

namespace sort {
    namespace partition {
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

            if (pivot < *ptr_left) {
                std::iter_swap(pivot_ptr, ptr_left);
                return ptr_left;
            }

            while (std::distance(ptr_left, end) > 1 && *ptr_left < pivot) {
                ++ptr_left;
            }
            std::iter_swap(ptr_left, pivot_ptr);
            return ptr_left;
        }
    }

    template<std::random_access_iterator It>
    inline void quicksort(It begin, It end) {
        if (std::distance(begin, end) < 2) return;
        It pivot = partition::default_partition(begin, end);
        quicksort(begin, pivot);
        quicksort(std::next(pivot), end);
    }

    SortableGenerator quicksort_worst_case_generator = [](size_t n) {
        Sortable sortable(n);
        std::iota(sortable.rbegin(), sortable.rend(), 0);
        return sortable;
    };

    template<typename It>
    void make_best_case(It begin, It end) {
        if (std::distance(begin, end) < 2) return;
        It middle = std::next(begin, std::distance(begin, end) / 2);
        make_best_case(begin, middle);
        make_best_case(std::next(middle), end);
        std::iter_swap(middle, std::prev(end));
    }

    SortableGenerator quicksort_best_case_generator = [](size_t n) {
        Sortable sortable(n);
        std::iota(sortable.begin(), sortable.end(), 0);
        make_best_case(sortable.begin(), sortable.end());
        return sortable;
    };
}


#endif //CODE_QUICK_SORT_V1_H