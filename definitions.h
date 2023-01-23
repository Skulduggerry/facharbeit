#ifndef CODE_DEFINITIONS_H
#define CODE_DEFINITIONS_H

#include <utility>
#include <vector>
#include <functional>
#include <map>
#include <chrono>
#include <string>
#include <memory>

#define DEFAULT_LOG_N_START_VALUE 6
#define DEFAULT_LOG_N_END_VALUE 16
#define DEFAULT_ITERATIONS_AVERAGE_CASE 15
#define DEFAULT_ITERATIONS_SPECIAL_CASE 5
#define MANY_EQUAL_OBJETS false

#define MIN_VALUE  0
#if MANY_EQUAL_OBJETS
#define MAX_VALUE (1ull << DEFAULT_LOG_N_END_VALUE / 2)
#else
#define MAX_VALUE (1ull << DEFAULT_LOG_N_END_VALUE)
#endif

namespace sort {
    enum Case {
        BEST_CASE, AVERAGE_CASE, WORST_CASE
    };

    using value = uint64_t;
    using Sortable = std::vector<value>;
    using Algorithm = std::function<void(Sortable::iterator, Sortable::iterator)>;
    using ExecutionResults = std::map<size_t, std::chrono::nanoseconds>;
    using SortableGenerator = std::function<std::unique_ptr<Sortable>(size_t)>;

    struct AlgorithmInformation {
        Algorithm algorithm_;
        std::string algorithmName_;
        std::map<Case, SortableGenerator> supportedSpecialCases;
        std::map<Case, ExecutionResults> perCaseResults;

        AlgorithmInformation(Algorithm algorithm,
                             std::string algorithmName,
                             const std::map<Case, SortableGenerator> &supportedSpacialCases) :
                algorithm_(std::move(algorithm)),
                algorithmName_(std::move(algorithmName)),
                supportedSpecialCases(supportedSpacialCases),
                perCaseResults{} {}
    };
}

#endif //CODE_DEFINITIONS_H