#ifndef CODE_DEFINITIONS_H
#define CODE_DEFINITIONS_H

#include <cstdint>
#include <vector>
#include <chrono>
#include <functional>
#include <map>
#include <memory>

#define DEFAULT_LOG_N_START 18
#define DEFAULT_LOG_N_END 18
#define ITERATIONS_AVERAGE_CASE 10
#define ITERATIONS_SPECIAL_CASE 3
#define MIN_VALUE 0

namespace sort {
    using clock = std::chrono::high_resolution_clock;
    using Value = uint64_t;
    using Sortable = std::vector<Value>;
    using Algorithm = std::function<void(Sortable::iterator, Sortable::iterator)>;
    using ExecutionResults = std::map<size_t, clock::duration>;
    using MaxValueFunction = std::function<Value(size_t log_n)>; //function to generate the max value for a given size
    using SortableGenerator = std::function<std::unique_ptr<Sortable>(size_t n,
                                                                      Value max_value)>; //max value is only required for average case

    enum Case {
        BEST_CASE, AVERAGE_CASE, WORST_CASE
    };

    struct AlgorithmInformation {
        Algorithm algorithm_;
        std::string algorithmName_;
        std::map<Case, SortableGenerator> supportedSpecialCases;
        std::map<Case, ExecutionResults> perCaseResults;

        AlgorithmInformation(Algorithm algorithm,
                             std::string algorithmName,
                             std::map<Case, SortableGenerator> supportedSpacialCases) :
                algorithm_(std::move(algorithm)),
                algorithmName_(std::move(algorithmName)),
                supportedSpecialCases(std::move(supportedSpacialCases)),
                perCaseResults{} {}
    };

    //generator function to generate many equal values
    Value MAX_VALUE_MANY_EQUAL(size_t log_n) {
        return 1ull << (log_n / 2);
    }

    //generator function to make most of the values different
    Value MAX_VALUE_MANY_DIFFERENT(size_t log_n) {
        return 1ull << log_n;
    }
}

#endif //CODE_DEFINITIONS_H