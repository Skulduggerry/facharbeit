#ifndef CODE_COUNTING_SORT_H
#define CODE_COUNTING_SORT_H

#include "definitions.h"

namespace sort {
    template<std::bidirectional_iterator It>
    inline void counting_sort(It begin, It end) {
        using ValueType = std::iterator_traits<It>::value_type;
        std::vector<ValueType> counts(std::distance(begin, end) + 1, 0);

        for (It i = begin; i != end; ++i)
            ++counts.at(*i);

        for (size_t number = 0; number < counts.size(); ++number) {
            for (size_t occurs = 0; occurs < counts.at(number); ++occurs) {
                *begin = number;
                ++begin;
            }
        }
    }
}

#endif //CODE_COUNTING_SORT_H
