#ifndef CODE_UTILS_H
#define CODE_UTILS_H

#include "definitions.h"
#include <random>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace sort {
    //generator for the average case
    SortableGenerator average_case_generator = [](uint64_t n, Value max_value) {
        static std::random_device engine{};
        std::uniform_int_distribution<Value> distribution{MIN_VALUE, max_value};

        Sortable sortable(n);
        for (uint64_t i = 0; i < n; ++i) {
            sortable[i] = distribution(engine);
        }
        return sortable;
    };

    SortableGenerator same_number_generator = [](uint64_t n, Value) {
        return Sortable(n, 42);
    };

    SortableGenerator sorted_number_generator = [](uint64_t n, Value) {
        Sortable sortable(n);
        std::iota(sortable.begin(), sortable.end(), 0);
        return sortable;
    };

    SortableGenerator reverse_sorted_number_generator = [](uint64_t n, Value) {
        Sortable sortable(n);
        std::iota(sortable.rbegin(), sortable.rend(), 0);
        return sortable;
    };

    inline std::ofstream
    create_output_file(const std::filesystem::path &parent_dir, const std::string &file_name, uint64_t log_n_start,
                       uint64_t log_n_end) {
        create_directories(parent_dir);
        std::ofstream output_file{parent_dir / (file_name + ".csv"),
                                  std::ios::trunc}; //create an empty file for the results
        if (output_file.fail()) {
            std::cerr << "Failed to open or create " + file_name + ".csv";
            std::abort();
        }

        output_file << ";";
        for (uint64_t log_n = log_n_start; log_n <= log_n_end; ++log_n) {
            output_file << log_n << ";";
        }
        output_file << '\n';

        return output_file;
    }

    inline void output_average_case(const std::vector<AlgorithmInformation> &algorithms,
                                    uint64_t log_n_start,
                                    uint64_t log_n_end,
                                    bool to_file, const std::filesystem::path &parent_dir) {
        std::ofstream average_case_results = create_output_file(parent_dir, "average_case_results",
                                                                log_n_start, log_n_end);
        for (const AlgorithmInformation &information: algorithms) {
            if (!information.perCaseResults_.contains(AVERAGE_CASE)) continue;
            average_case_results << information.algorithmName_ << ";";
            for (const auto &[log_n, duration]: information.perCaseResults_.at(AVERAGE_CASE)) {
                average_case_results << duration.count() << ";";
            }
            average_case_results << '\n';
        }
    }

    inline void
    output(const std::vector<AlgorithmInformation> &algorithms,
           uint64_t log_n_start,
           uint64_t log_n_end,
           bool to_file, const std::filesystem::path &parent_dir) {
        if (to_file) {
            //write results to file
            using namespace std::filesystem;
            create_directories(parent_dir);

            bool contains_average_case = false;

            for (auto &[algorithm, algorithm_name, executed_cases, per_case_results]: algorithms) {
                std::ofstream algorithm_results = create_output_file(parent_dir, algorithm_name,
                                                                     log_n_start, log_n_end);

                for (auto &[case_, results]: per_case_results) {
                    if (case_ == AVERAGE_CASE) contains_average_case = true;

                    algorithm_results << case_ << ";";
                    for (auto &[log_n, duration]: results) {
                        algorithm_results << duration.count() << ";";
                    }
                    algorithm_results << '\n';
                }
            }

            //write average cases
            if (contains_average_case) {
                output_average_case(algorithms, log_n_start, log_n_end, to_file, parent_dir);
            }
        }

        //write results to console
        for (auto &[algo, name, executed_cases, per_case_results]: algorithms) {
            std::cout << name << ":\n";
            for (auto &[case_, results]: per_case_results) {
                std::cout << "  " << case_ << ":\n";
                for (auto &[log_n, required_time]: results) {
                    std::cout << "    " << std::setw(2) << log_n << " : " << std::setw(16) << required_time.count()
                              << "ns\n";
                }
            }
        }
    }

    inline ExecutionResults evaluate(const SortableGenerator &generator,
                                     const MaxValueFunction &max_value_function,
                                     const Algorithm &algorithm,
                                     const std::string &algorithm_name,
                                     const uint64_t log_n_start,
                                     const uint64_t log_n_end,
                                     const uint64_t iterations) {
        ExecutionResults results{};

        for (uint64_t log_n = log_n_start; log_n <= log_n_end; ++log_n) {
            clock::duration total_duration{};

            Value n = 1ull << log_n;
            Value max_value = max_value_function(n);
            for (uint64_t iteration = 0; iteration < iterations; ++iteration) {
                auto sortable = generator(n, max_value);
                auto start_point = clock::now();
                algorithm(sortable.begin(), sortable.end());
                auto end_point = clock::now();
                total_duration += (end_point - start_point);

                if (!std::is_sorted(sortable.begin(), sortable.end())) {
                    std::cerr << "Algorithm " << algorithm_name << " does not sort correctly at log_n=" << log_n
                              << '\n';
                    std::abort();
                }
            }

            total_duration /= iterations;
            results[log_n] = total_duration;
        }

        return results;
    }

    inline void benchmark(std::vector<AlgorithmInformation> &algorithms,
                          const MaxValueFunction &max_value_function,
                          const uint64_t log_n_start,
                          const uint64_t log_n_end,
                          bool to_file,
                          const std::filesystem::path &parent_dir = "./results") {

        for (auto &[algorithm, name, executed_cases, per_case_results]: algorithms) {
            for (auto &[case_, case_execution_information]: executed_cases) {
                auto &[iterations, generator] = case_execution_information;
                ExecutionResults case_result = evaluate(generator, max_value_function, algorithm, name, log_n_start,
                                                        log_n_end, iterations);
                per_case_results[case_] = case_result;
            }
        }

        output(algorithms, log_n_start, log_n_end, to_file, parent_dir);
    }
}

#endif //CODE_UTILS_H