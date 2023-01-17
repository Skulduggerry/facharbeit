#ifndef CODE_UTILS_H
#define CODE_UTILS_H

#include "definitions.h"
#include <random>
#include <chrono>
#include <iostream>

namespace sort {
    inline Sortable generate_average_case(const size_t n) {
        std::random_device engine{};
        std::uniform_int_distribution<value> distribution{0, static_cast<value>(-1)};
        Sortable result(n);
        for (size_t i = 0; i < n; ++i)
            result[i] = distribution(engine);
        return result;
    }

    inline ExecutionResults
    evaluate(const SortableGenerator &generator, const Algorithm &algo, const std::string &algo_name,
             const size_t log_n_start = DEFAULT_LOG_N_START_VALUE,
             const size_t log_n_end = DEFAULT_LOG_N_END_VALUE,
             const size_t iterations = DEFAULT_ITERATIONS_SPECIAL_CASE) {
        ExecutionResults result{};

        for (size_t log_n = log_n_start; log_n <= log_n_end; ++log_n) {
            std::chrono::nanoseconds time{};
            for (size_t iteration = 0; iteration < iterations; ++iteration) {
                Sortable sortable = generator(1ull << log_n);
                auto start = std::chrono::high_resolution_clock::now();
                algo(sortable.begin(), sortable.end());
                auto end = std::chrono::high_resolution_clock::now();
                time += (end - start);

                if (!std::is_sorted(sortable.begin(), sortable.end())) {
                    std::cerr << "Algorithm " << algo_name << " does not work correct!" << std::endl;
                    std::abort();
                }
            }
            time /= iterations; //get the average required time
            result.emplace(log_n, time);
        }

        return result;
    }

    inline ExecutionResults evaluate_average_case(const Algorithm &algo, const std::string &algo_name,
                                                  const size_t log_n_start = DEFAULT_LOG_N_START_VALUE,
                                                  const size_t log_n_end = DEFAULT_LOG_N_END_VALUE,
                                                  const size_t iterations = DEFAULT_ITERATIONS_AVERAGE_CASE) {
        return evaluate(generate_average_case, algo, algo_name, log_n_start, log_n_end, iterations);
    }

    inline std::string to_string(const Case &case_) {
        switch (case_) {
            case BEST_CASE:
                return "BEST_CASE   ";
            case AVERAGE_CASE:
                return "AVERAGE_CASE";
            default:
                return "WORST_CASE  ";
        }
    }

    inline void
    add_results(SizeCaseResults &size_case_results,
                const ExecutionResults &case_results,
                const Case &case_) {
        for (const auto &[log_n, required_time]: case_results)
            size_case_results[log_n].emplace(case_, required_time);
    }

    inline void benchmark(std::vector<AlgorithmInformation> &algorithm_information) {
        using namespace sort;

        for (auto &[algorithm, name, special_cases, per_case_size_results]: algorithm_information) {
            for (auto &[case_, generator]: special_cases) {
                ExecutionResults special_case_result = evaluate(generator, algorithm, name);

                add_results(per_case_size_results, special_case_result, case_);
            }

            ExecutionResults average_case_results = evaluate_average_case(algorithm, name);
            add_results(per_case_size_results, average_case_results, AVERAGE_CASE);
        }
    }

    inline void output(const std::vector<AlgorithmInformation> & algorithm_information) {
        for (auto &information: algorithm_information) {
            std::cout << information.algorithmName_ << ":\n";
            for (auto &[log_n, perCaseResults]: information.sizeCaseResults_) {
                std::cout << "  log_n: " << log_n << '\n';
                for (auto &[case_, required_time]: perCaseResults) {
                    std::cout << "    " << to_string(case_) << ": " << required_time.count() << "ns\n";
                }
            }
        }
    }
}


#endif //CODE_UTILS_H