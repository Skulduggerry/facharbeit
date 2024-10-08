#ifndef CODE_DEFINITIONS_H
#define CODE_DEFINITIONS_H

#include <functional>
#include <map>
#include <cstdint>
#include <chrono>

#define DEFAULT_ITERATIONS_AVERAGE_CASE 10
#define DEFAULT_ITERATIONS_SPECIAL_CASE 3
#define DEFAULT_LOG_N_START 7
#define DEFAULT_LOG_N_END 15
#define MIN_VALUE 0
#define DEFAULT_PERCENTAGE_EQUAL_VALUES 25
#define TO_PRIMITIVE_THRESHOLD 8

namespace sort {
    using clock = std::chrono::high_resolution_clock;
    using Value = uint64_t;
    using Sortable = std::vector<Value>;
    using Algorithm = std::function<void(Sortable::iterator, Sortable::iterator)>;
    using ExecutionResults = std::map<uint64_t, clock::duration>;
    using MaxValueFunction = std::function<Value(uint64_t n)>; //function to generate the max value for a given size
    using SortableGenerator = std::function<Sortable(uint64_t n, Value max_value)>; //max value is only required for average case
    using CaseExecutionInformation = std::pair<uint64_t, SortableGenerator>; //store the iterations and the generator
    const std::string BEST_CASE = "BEST_CASE";
    const std::string AVERAGE_CASE = "AVERAGE_CASE";
    const std::string WORST_CASE = "WORST_CASE";

    struct AlgorithmInformation {
        Algorithm algorithm_;
        std::string algorithmName_;
        std::map<std::string, CaseExecutionInformation> executedCases_;
        std::map<std::string, ExecutionResults> perCaseResults_;

        AlgorithmInformation(Algorithm algorithm,
                             std::string algorithmName,
                             std::map<std::string, CaseExecutionInformation> executedCases) :
                algorithm_(std::move(algorithm)),
                algorithmName_(std::move(algorithmName)),
                executedCases_(std::move(executedCases)),
                perCaseResults_{} {}
    };

    //generator function to generate many equal values
    Value MAX_VALUE_MANY_EQUAL(uint64_t n, long double percentage_equal) {
        return Value(1 / (percentage_equal / double(100)));
    }

    //generator function to make most of the values different
    Value MAX_VALUE_MANY_DIFFERENT(uint64_t) {
        return Value(-1);
    }
}

#endif //CODE_DEFINITIONS_H