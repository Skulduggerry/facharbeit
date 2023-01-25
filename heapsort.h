#ifndef CODE_HEAPSORT_H
#define CODE_HEAPSORT_H

#include "definitions.h"

namespace sort {

    template<std::random_access_iterator It>
    inline void adjust_heap(It begin_heap, It parent, It end_heap) {
        It child = parent + (parent - begin_heap) + 1;

        //go the heap down and swap with the greatest child if required
        while (child < end_heap) {
            if ((child + 1) < end_heap && *child < *(child + 1)) {
                ++child;
            }

            if (*parent < *child) {
                std::iter_swap(parent, child);
                parent = child;
                child = parent + (parent - begin_heap) + 1;
            } else break;
        }
    }

    template<std::random_access_iterator It>
    inline void build_heap(It begin, It end) {
        It parent = begin + (end - begin - 2) / 2;
        for (; parent >= begin; --parent) {
            adjust_heap(begin, parent, end);
        }
    }

    template<std::random_access_iterator It>
    inline void heapsort(It begin, It end) {
        build_heap(begin, end);
        for (It i = std::prev(end); std::distance(begin, i) > 0; --i) {
            std::iter_swap(begin, i); //put the new greatest value to the beginning of the sorted array
            adjust_heap(begin, begin, i);
        }
    }

    SortableGenerator heapsort_best_case_generator = [](size_t n, Value) {
        return std::make_unique<Sortable>(n, 0);
    };
}

#endif //CODE_HEAPSORT_H