#include <vector>
#include "quicksort.h"
#include "counting_sort.h"
#include "heapsort.h"
#include "insertion_sort.h"
#include "utils.h"

class ArgumentParser {
public:
    ArgumentParser(int argc, char *argv[]) {
        for (int i = 0; i < argc; ++i)
            arguments.emplace_back(*(argv + i));
    }

    bool has_argument(const std::string &arg) {
        return std::find(arguments.begin(), arguments.end(), arg) != arguments.end();
    }

    std::string get_argument(const std::string &arg, std::string def) {
        auto it = std::find(arguments.begin(), arguments.end(), arg);
        if (it != arguments.end() && ++it != arguments.end())
            return *it;
        return def;
    }

    size_t get_ull_argument(const std::string &arg, size_t def) {
        auto it = std::find(arguments.begin(), arguments.end(), arg);
        if (it != arguments.end() && ++it != arguments.end()) {
            try {
                return std::stoull(*it);
            } catch (...) {
                std::cerr << "'" << *it << "' is not a valid input for argument '" << arg << "'!";
                std::abort();
            }
        }
        return def;
    }

private:
    std::vector<std::string> arguments{};
};

std::vector<std::string> split(const std::string &s, const std::string &delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}

std::pair<size_t, sort::SortableGenerator> to_special_case(const sort::SortableGenerator &generator) {
    return std::make_pair(DEFAULT_ITERATIONS_SPECIAL_CASE, generator);
}

int main(int argc, char *argv[]) {
    using namespace sort;
    //list of all supported algorithms with their special cases
    std::vector<AlgorithmInformation> algorithms = {
            {
                    quicksort<Sortable::iterator>,              "quicksort",
                    {
                            {BEST_CASE, to_special_case(quicksort_best_case_generator)},
                            {WORST_CASE, to_special_case(quicksort_worst_case_generator)}
                    }
            },
            {
                    quick_sort_median_of_3<Sortable::iterator>, "quicksort_median_of_three",
                    {
                            {BEST_CASE, to_special_case(quicksort_worst_case_generator)},
                            {WORST_CASE, to_special_case(same_number_generator)}
                    }
            },
            {
                    three_way_quicksort<Sortable::iterator>,    "three_way_quicksort",
                    {
                            {BEST_CASE, to_special_case(same_number_generator)},
                            {WORST_CASE, to_special_case(quicksort_worst_case_generator)}
                    }
            },
            {
                    counting_sort<Sortable::iterator>,          "counting_sort",
                    {}
            },
            {
                    heapsort<Sortable::iterator>,               "heapsort",
                    {
                            {BEST_CASE, to_special_case(heapsort_best_case_generator)}
                    }
            },
            {
                    insertion_sort<Sortable::iterator>,         "insertion_sort",
                    {
                            {BEST_CASE, to_special_case(insertion_sort_best_case_generator)},
                            {WORST_CASE, to_special_case(insertion_sort_worst_case_generator)}
                    }
            }
    };

    ArgumentParser parser{argc, argv};

    //send help
    if (parser.has_argument("-h") || parser.has_argument("-help")) {
        std::cout << "'-tf'  and '-to_file'                              : write the results also to a file\n";
        std::cout << "'-md'  and '-many_different'                       : use many different values\n";
        std::cout << "'-me'  and '-many_equal'                           : use many equal values\n";
        std::cout << "'-lns' and '-log_n_start'                          : set start number of elements\n";
        std::cout << "'-lne' and '-log_n_end'                            : set inclusive number of elements\n";
        std::cout << "'-isc' and '-iterations_special_case' <iterations> : set iterations used for special cases (0 to disable)\n";
        std::cout << "'-iac' and '-iterations_average_case' <iterations> : set iterations used for average case (0 to disable)\n";
        std::cout << "'-dbc' and '-disable_best_case'                    : don't benchmark the best case\n";
        std::cout << "'-dac' and '-disable_average_case'                 : don't benchmark the average case\n";
        std::cout << "'-dwc' and '-disable_worst_case'                   : don't benchmark the worst case\n";
        std::cout << "'-ra' and  '-remove_algorithm' <algorithms>        : don't benchmark the given algorithms (seperated by '|')\n";
        std::cout << "'-h'  and  '-help'                                 : get this help message\n";

        std::cout << "\n-----------------------\n";
        std::cout << "Supported Algorithms:\n";
        std::cout << "-----------------------\n";
        std::for_each(algorithms.begin(), algorithms.end(), [](auto &algorithm) {
            std::cout << algorithm.algorithmName_ << '\n';
        });

        return 0;
    }

    //find all required information
    bool to_file = parser.has_argument("-tf") || parser.has_argument("-to_file");
    std::string root_path = parser.get_argument("-path", parser.get_argument("-p", "./results"));
    bool many_different = parser.has_argument("-md") || parser.has_argument("-many_different");
    bool many_equal = parser.has_argument("-me") || parser.has_argument("-many_equal");
    size_t log_n_start = parser.get_ull_argument("-lns", parser.get_ull_argument("log_n_start", DEFAULT_LOG_N_START));
    size_t log_n_end = parser.get_ull_argument("-lne", parser.get_ull_argument("-log_n_end", DEFAULT_LOG_N_END));

    size_t iterations_special_case = parser.get_ull_argument("-isc", parser.get_ull_argument("-iterations_special_case", DEFAULT_ITERATIONS_SPECIAL_CASE));
    bool disable_best_case = parser.has_argument("-dbc") || parser.has_argument("-disable_best_case") || iterations_special_case == 0;
    bool disable_worst_case = parser.has_argument("-dwc") || parser.has_argument("-disable_worst_case") || iterations_special_case == 0;
    size_t iterations_average_case = parser.get_ull_argument("-iac", parser.get_ull_argument("-iterations_average_case", DEFAULT_ITERATIONS_AVERAGE_CASE));
    bool disable_average_case = parser.has_argument("-dac") || parser.has_argument("-disable_average_case") || iterations_average_case == 0;

    //remove all best cases
    if (disable_best_case) {
        for (auto &algorithm: algorithms) {
            algorithm.executedCases_.erase(BEST_CASE);
        }
    }

    //remove all worst cases
    if (disable_worst_case) {
        for (auto &algorithm: algorithms) {
            algorithm.executedCases_.erase(WORST_CASE);
        }
    }

    //set the new iterations for special cases
    if (0 != iterations_special_case && DEFAULT_ITERATIONS_SPECIAL_CASE != iterations_special_case) {
        for (auto &algorithm: algorithms) {
            for (auto &[case_, information]: algorithm.executedCases_) {
                information.first = iterations_special_case;
            }
        }
    }

    //add average case
    if (!disable_average_case) {
        for (auto &algorithm: algorithms) {
            algorithm.executedCases_[AVERAGE_CASE] = std::make_pair(iterations_average_case, average_case_generator);
        }
    }


    //remove algorithms
    std::vector<std::string> remove_algorithms = split(parser.get_argument("-ra", parser.get_argument("-remove_algorithm", "")), "|");
    auto new_end = std::remove_if(algorithms.begin(), algorithms.end(), [&](const auto &algorithm) {
        return std::find(remove_algorithms.begin(), remove_algorithms.end(), algorithm.algorithmName_) != remove_algorithms.end();
    });
    algorithms.erase(new_end, algorithms.end());

    //execute add minimum one of them
    if (many_different || many_different == many_equal) {
        benchmark(algorithms, MAX_VALUE_MANY_DIFFERENT, log_n_start, log_n_end, to_file, root_path + "/many_different");
    }
    if (many_equal || many_equal == many_different) {
        benchmark(algorithms, MAX_VALUE_MANY_EQUAL, log_n_start, log_n_end, to_file, root_path + "/many_equal");
    }
}