#ifndef CODE_DEFINITIONS_H
#define CODE_DEFINITIONS_H

#include <utility>
#include <vector>
#include <functional>
#include <map>
#include <chrono>
#include <string>

#define DEFAULT_LOG_N_START_VALUE 10
#define DEFAULT_LOG_N_END_VALUE 15
#define DEFAULT_ITERATIONS_AVERAGE_CASE 20
#define DEFAULT_ITERATIONS_SPECIAL_CASE 5

namespace sort {
    enum Case {
        BEST_CASE, AVERAGE_CASE, WORST_CASE
    };

    using value = uint16_t;
    using Sortable = std::vector<value>;
    using Algorithm = std::function<void(Sortable::iterator, Sortable::iterator)>;
    using ExecutionResults = std::map<size_t, std::chrono::nanoseconds>;
    using CaseResults = std::map<Case, std::chrono::nanoseconds>;
    using SizeCaseResults = std::map<size_t, CaseResults>;
    using SortableGenerator = std::function<Sortable(size_t)>;

    struct AlgorithmInformation {
        Algorithm algorithm_;
        std::string algorithmName_;
        std::map<Case, SortableGenerator> supportedSpecialCases;
        SizeCaseResults sizeCaseResults_;

        AlgorithmInformation(Algorithm algorithm,
                             std::string algorithmName,
                             const std::map<Case, SortableGenerator> &supportedSpacialCases) :
                algorithm_(std::move(algorithm)),
                algorithmName_(std::move(algorithmName)),
                supportedSpecialCases(supportedSpacialCases),
                sizeCaseResults_{} {}
    };
}

#endif //CODE_DEFINITIONS_H