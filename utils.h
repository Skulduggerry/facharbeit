#ifndef CODE_UTILS_H
#define CODE_UTILS_H

#include "definitions.h"
#include <random>
#include <chrono>
#include <iostream>
#include <fstream>
#include <set>
#include <iomanip>

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
                return "BEST_CASE";
            case AVERAGE_CASE:
                return "AVERAGE_CASE";
            default:
                return "WORST_CASE";
        }
    }

    inline void benchmark(std::vector<AlgorithmInformation> &algorithm_information) {
        using namespace sort;

        if (algorithm_information.empty()) return;
        evaluate_average_case(algorithm_information.at(0).algorithm_, "---", 15);

        for (auto &[algorithm, name, special_cases, per_case_results]: algorithm_information) {
            for (auto &[case_, generator]: special_cases) {
                ExecutionResults special_case_result = evaluate(generator, algorithm, name);

                per_case_results[case_] = special_case_result;
            }

            ExecutionResults average_case_results = evaluate_average_case(algorithm, name);
            per_case_results[AVERAGE_CASE] = average_case_results;
        }
    }

    inline void output(const std::vector<AlgorithmInformation> &algorithm_information, bool to_file = false) {
        if (to_file) {
            if (algorithm_information.empty()) return;

            std::ofstream average_case_result_file{"average_case_results.csv", std::ios::trunc};
            if (average_case_result_file.fail()) {
                std::cerr << "Failed to open or create average_case_results.csv";
                std::abort();
            }

            std::set<size_t> logs{};
            for (auto &information: algorithm_information) {
                for (auto &[case_, results]: information.perCaseResults) {
                    for (auto &[log_n, time]: results) {
                        logs.emplace(log_n);
                    }
                }
            }

            average_case_result_file << ";";
            for (auto &log_n: logs) {
                average_case_result_file << log_n << ";";
            }
            average_case_result_file << std::endl;

            for (auto &[algo, name, supported, per_case_results]: algorithm_information) {
                std::ofstream algorithm_result_file{name + "_results.csv", std::ios::trunc};
                if (algorithm_result_file.fail()) {
                    std::cerr << "Failed to open or create " << name << "_results.csv";
                    std::abort();
                }

                algorithm_result_file << ";";
                for (auto &log_n: logs) {
                    algorithm_result_file << log_n << ";";
                }
                algorithm_result_file << std::endl;

                for (auto &[case_, results]: per_case_results) {
                    algorithm_result_file << to_string(case_) << ";";
                    if (AVERAGE_CASE == case_) average_case_result_file << name << ";";
                    for (auto &[log_n, required_time]: results) {
                        std::string output = logs.contains(log_n) ? std::to_string(required_time.count()) + ";" : ";";
                        algorithm_result_file << output;
                        if (AVERAGE_CASE == case_) average_case_result_file << output;
                    }
                    algorithm_result_file << std::endl;
                    if (AVERAGE_CASE == case_) average_case_result_file << std::endl;
                }
            }
        }

        for (auto &[algo, name, supported, per_case_results]: algorithm_information) {
            std::cout << name << ":\n";
            for (auto &[case_, results]: per_case_results) {
                std::cout << "  " << to_string(case_) << ":\n";
                for (auto &[log_n, required_time]: results) {
                    std::cout << "    " << log_n << " : " << std::setw(15) << required_time.count() << "ns\n";
                }
            }
        }
    }
}


#endif //CODE_UTILS_H