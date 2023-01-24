#ifndef CODE_UTILS_H
#define CODE_UTILS_H

#include "definitions.h"
#include <random>
#include <filesystem>
#include <iostream>
#include <fstream>

namespace sort {
    //generator for the average case
    SortableGenerator average_case_generator = [](size_t n, Value max_value) {
        static std::random_device engine{};
        std::uniform_int_distribution<Value> distribution{MIN_VALUE, max_value};

        std::unique_ptr<Sortable> sortable_ptr = std::make_unique<Sortable>(n);
        for (size_t i = 0; i < n; ++i) {
            (*sortable_ptr)[i] = distribution(engine);
        }
        return sortable_ptr;
    };

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

    inline std::ofstream
    create_output_file(const std::filesystem::path &parent_dir, const std::string &file_name, size_t log_n_start,
                       size_t log_n_end) {
        create_directories(parent_dir);
        std::ofstream output_file{parent_dir / (file_name + ".csv"),std::ios::trunc}; //create an empty file for the results
        if (output_file.fail()) {
            std::cerr << "Failed to open or create " + file_name + ".csv";
            std::abort();
        }

        output_file << ";";
        for (size_t log_n = log_n_start; log_n <= log_n_end; ++log_n) {
            output_file << log_n << ";";
        }
        output_file << '\n';

        return std::move(output_file);
    }

    inline void
    output(const std::vector<AlgorithmInformation> &algorithms,
           size_t log_n_start,
           size_t log_n_end,
           bool to_file, const std::filesystem::path &parent_dir) {
        if (to_file) {
            //write results to file
            using namespace std::filesystem;
            create_directories(parent_dir);

            std::ofstream average_case_results = create_output_file(parent_dir, "average_case_results",
                                                                    log_n_start, log_n_end);

            for (auto &[algorithm, algorithm_name, special_cases, per_case_results]: algorithms) {
                std::ofstream algorithm_results = create_output_file(parent_dir, algorithm_name,
                                                                     log_n_start, log_n_end);

                for (auto &[case_, results]: per_case_results) {
                    algorithm_results << to_string(case_) << ";";
                    if (AVERAGE_CASE == case_) algorithm_results << algorithm_name << ";";
                    for (auto &[log_n, required_time]: results) {
                        algorithm_results << required_time.count() << ";";
                        if (AVERAGE_CASE == case_) average_case_results << required_time.count() << ";";
                    }
                    algorithm_results << std::endl;
                    if (AVERAGE_CASE == case_) average_case_results << std::endl;
                }
            }
        }

        //write results to console
        for (auto &[algo, name, supported, per_case_results]: algorithms) {
            std::cout << name << ":\n";
            for (auto &[case_, results]: per_case_results) {
                std::cout << "  " << to_string(case_) << ":\n";
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
                                     const size_t log_n_start,
                                     const size_t log_n_end,
                                     const size_t iterations = ITERATIONS_SPECIAL_CASE) {
        ExecutionResults results{};

        for (size_t log_n = log_n_start; log_n <= log_n_end; ++log_n) {
            clock::duration total_duration{};

            Value max_value = max_value_function(log_n);
            for (size_t iteration = 0; iteration < iterations; ++iteration) {
                auto sortable = generator(1ull << log_n, max_value);
                auto start_point = clock::now();
                algorithm(sortable->begin(), sortable->end());
                auto end_point = clock::now();
                total_duration += (end_point - start_point);

                if (!std::is_sorted(sortable->begin(), sortable->end())) {
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

    inline ExecutionResults evaluate_average_case(const MaxValueFunction &max_value_function,
                                                  const Algorithm &algorithm,
                                                  const std::string &algorithm_name,
                                                  const size_t log_n_start,
                                                  const size_t log_n_end) {
        return evaluate(average_case_generator, max_value_function, algorithm, algorithm_name, log_n_start, log_n_end,
                        ITERATIONS_AVERAGE_CASE);
    }

    void benchmark(std::vector<AlgorithmInformation> &algorithms,
                   const MaxValueFunction &max_value_function,
                   const size_t log_n_start,
                   const size_t log_n_end,
                   bool to_file,
                   const std::filesystem::path &parent_dir = "./results") {

        for (auto &[algorithm, name, special_cases, per_case_results]: algorithms) {
            for (auto &[case_, generator]: special_cases) {
                ExecutionResults special_case_result = evaluate(generator, max_value_function, algorithm, name,
                                                                log_n_start, log_n_end);
                per_case_results[case_] = special_case_result;
            }

            ExecutionResults average_case_results = evaluate_average_case(max_value_function, algorithm, name,
                                                                          log_n_start, log_n_end);
            per_case_results[AVERAGE_CASE] = average_case_results;
        }

        output(algorithms, log_n_start, log_n_end, to_file, parent_dir);
    }
}

#endif //CODE_UTILS_H