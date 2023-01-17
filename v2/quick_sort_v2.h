#ifndef CODE_QUICK_SORT_V2_H
#define CODE_QUICK_SORT_V2_H

#include "../definitions.h"
#include "../v1/quick_sort_v1.h"
#include <numeric>
#include <cmath>
#include <iostream>

namespace sort {
    namespace partition {
        template<std::random_access_iterator It>
        inline It random_partition(It begin, It end) {
            auto random_pos = rand() % (std::distance(begin, end) + 1);
            std::iter_swap(std::next(begin, random_pos), std::prev(end));
            return default_partition(begin, end);
        }
    }

    template<std::random_access_iterator It>
    inline void quicksort_iterative(It begin, It end) {
        auto max_stack_size = static_cast<size_t>(2 * std::ceil(log2(std::distance(begin, end))));
        std::vector<std::pair<It, It>> call_chain{};
        call_chain.emplace_back(begin, end);
        call_chain.resize(max_stack_size);

        do {
            auto [begin_, end_] = call_chain.back();
            call_chain.pop_back();
            if (std::distance(begin_, end_) < 2) continue;

            It pivot = partition::default_partition(begin_, end_);
            call_chain.emplace_back(begin_, pivot);
            call_chain.emplace_back(std::next(pivot), end_);
        } while (!call_chain.empty());
    }

    template<std::random_access_iterator It>
    inline void quicksort_random_partition(It begin, It end) {
        if (std::distance(begin, end) < 2) return;
        It pivot = partition::random_partition(begin, end);
        quicksort(begin, pivot);
        quicksort(std::next(pivot), end);
    }
}


#endif //CODE_QUICK_SORT_V2_H