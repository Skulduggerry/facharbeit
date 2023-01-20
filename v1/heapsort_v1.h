#ifndef CODE_HEAPSORT_V1_H
#define CODE_HEAPSORT_V1_H

#include "../definitions.h"

namespace sort {

    template<std::random_access_iterator It>
    void adjust_heap(It begin_heap, It parent, It end_heap) {
        It child = parent + (parent - begin_heap) + 1;

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
    void build_heap(It begin, It end) {
        It parent = begin + (end - begin - 2) / 2;
        for (; parent >= begin; --parent) {
            adjust_heap(begin, parent, end);
            std::cout << "";
        }
    }

    template<std::random_access_iterator It>
    void heapsort(It begin, It end) {
        build_heap(begin, end);
        for (It i = std::prev(end); std::distance(begin, i) > 0; --i) {
            std::iter_swap(begin, i);
            adjust_heap(begin, begin, i);
        }
    }
}

#endif //CODE_HEAPSORT_V1_H
