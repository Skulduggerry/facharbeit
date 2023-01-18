#ifndef CODE_MERGE_SORT_H

#include "../definitions.h"

namespace sort {
    template<std::random_access_iterator It>
    void merge(It begin, It middle, It end) {
        std::vector vector_left(begin, middle);

        It ptr_original_vector = begin;
        It ptr_vector_left = vector_left.begin();
        It ptr_vector_right = middle;

        while (ptr_vector_left != vector_left.end() && ptr_vector_right != end) {
            if (*ptr_vector_left <= *ptr_vector_right) {
                *ptr_original_vector = *ptr_vector_left;
                std::advance(ptr_original_vector, 1);
                std::advance(ptr_vector_left, 1);
            } else {
                *ptr_original_vector = *ptr_vector_right;
                std::advance(ptr_original_vector, 1);
                std::advance(ptr_vector_right, 1);
            }
        }

        while (ptr_vector_left != vector_left.end()) {
            *ptr_original_vector = *ptr_vector_left;
            std::advance(ptr_original_vector, 1);
            std::advance(ptr_vector_left, 1);
        }
    }

    template<std::random_access_iterator It>
    void merge_sort(It begin, It end) {
        auto distance = std::distance(begin, end);
        if (2 > distance) return;
        It middle = std::next(begin, distance / 2);
        merge_sort(begin, middle);
        merge_sort(middle, end);
        merge(begin, middle, end);
    }
}

#define CODE_MERGE_SORT_H

#endif //CODE_MERGE_SORT_H
