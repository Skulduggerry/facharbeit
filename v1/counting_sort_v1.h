#ifndef CODE_COUNTING_SORT_V1_H
#define CODE_COUNTING_SORT_V1_H

#include "../definitions.h"

namespace sort {
    template<std::bidirectional_iterator It, size_t max_value>
    void counting_sort(It begin, It end) {
        using Value = std::iterator_traits<It>::value_type;
        std::array<Value, max_value + 1> counts{};
        counts.fill(0);

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

#endif //CODE_COUNTING_SORT_V1_H
