#include "quicksort.h"
#include "heapsort.h"
#include "insertion_sort.h"
#include "intro_sort.h"
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

    uint64_t get_ull_argument(const std::string &arg, uint64_t def) {
        auto param = get_argument(arg, std::to_string(def));
        try {
            return std::stoull(param);
        } catch (...) {
            std::cerr << "'" << param << "' is not a valid input for argument '" << arg << "'!";
            return def;
        }
    }

    long double get_ld_argument(const std::string &arg, long double def) {
        auto param = get_argument(arg, std::to_string(def));
        try {
            return std::stold(param);
        } catch (...) {
            std::cerr << "'" << param << "' is not a valid input for argument '" << arg << "'!";
            return def;
        }
    }

private:
    std::vector<std::string> arguments{};
};

std::vector<std::string> split(const std::string &s, const std::string &delimiter) {
    uint64_t pos_start = 0, pos_end, delim_len = delimiter.length();
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

std::pair<std::string, std::pair<uint64_t, sort::SortableGenerator>> to_special_case(const std::string &name, const sort::SortableGenerator &generator) {
    return std::make_pair(name, std::make_pair(DEFAULT_ITERATIONS_AVERAGE_CASE, generator));
}

std::pair<std::string, std::pair<uint64_t, sort::SortableGenerator>> to_best_case(const sort::SortableGenerator &generator) {
    return to_special_case(sort::BEST_CASE, generator);
}

std::pair<std::string, std::pair<uint64_t, sort::SortableGenerator>> to_worst_case(const sort::SortableGenerator &generator) {
    return to_special_case(sort::WORST_CASE, generator);
}

int main(int argc, char *argv[]) {
    using namespace sort;
    //list of all supported algorithms with their special cases
    std::vector<AlgorithmInformation> algorithms = {
            {
                    quicksort<Sortable::iterator>,              "quicksort",
                    {
                            to_best_case(quicksort_best_case_generator),
                            to_special_case("WORST_CASE_SORTED", quicksort_worst_case_generator),
                            to_special_case("WORST_CASE_ALL_SAME", same_number_generator)
                    }
            },
            {
                    quick_sort_median_of_3<Sortable::iterator>, "quicksort_median_of_three",
                    {
                            to_best_case(quicksort_worst_case_generator),
                            to_worst_case(same_number_generator)
                    }
            },
            {
                    three_way_quicksort<Sortable::iterator>,    "three_way_quicksort",
                    {
                            to_best_case(same_number_generator),
                            to_worst_case(quicksort_worst_case_generator)
                    }
            },
            {
                    heapsort<Sortable::iterator>,               "heapsort",
                    {
                            to_best_case(same_number_generator)
                    }
            },
            {
                    insertion_sort<Sortable::iterator>,         "insertion_sort",
                    {
                            to_special_case("BEST_CASE_SORTED", sorted_number_generator),
                            to_special_case("BEST_CASE_ALL_SAME", same_number_generator),
                            to_worst_case(reverse_sorted_number_generator)
                    }
            },
            {
                    intro_sort<Sortable::iterator>,             "intro_sort",
                    {}
            },
            {
                    std::sort<Sortable::iterator>,              "std_sort",
                    {}
            }
    };

    ArgumentParser parser{argc, argv};

    //send help
    if (parser.has_argument("-h") || parser.has_argument("-help")) {
        std::cout << "'-tf'  and '-to_file'                              : write the results also to result file\n";
        std::cout << "'-p'   and '-path'                                 : set the root path for result files\n";

        std::cout << "'-md'  and '-many_different'                       : use many different values (might not effect best- or worst-case)\n";
        std::cout << "'-me'  and '-many_equal'                           : use many equal values     (might not effect best- or worst-case)\n";
        std::cout << "'-mep' and '-many_equal_percentage'                : set percentage of equal values when executing '-me' (must be between 0 and 50 or it will be clamped to the closest value)\n";
        std::cout << "'-lns' and '-log_n_start'                          : set start number of elements\n";
        std::cout << "'-lne' and '-log_n_end'                            : set inclusive number of elements\n";

        std::cout << "'-isc' and '-iterations_special_case' <iterations> : set iterations used for special cases (0 to disable)\n";
        std::cout << "'-iac' and '-iterations_average_case' <iterations> : set iterations used for average case (0 to disable)\n";

        std::cout << "'-dbc' and '-disable_best_case'                    : don't benchmark the best case\n";
        std::cout << "'-dac' and '-disable_average_case'                 : don't benchmark the average case\n";
        std::cout << "'-dwc' and '-disable_worst_case'                   : don't benchmark the worst case\n";
        std::cout << "'-ea'  and '-enable_algorithm'   <algorithms>      : benchmark only the given algorithms  (seperated by '|') (cannot be used with '-da')\n";
        std::cout << "'-da'  and '-disable_algorithm' <algorithms>       : don't benchmark the given algorithms (seperated by '|') (cannot be used with '-ea')\n";
        std::cout << "'-h'   and '-help'                                 : get this help message\n";

        std::cout << "\n-----------------------\n";
        std::cout << "Supported Algorithms:\n";
        std::cout << "-----------------------\n";
        std::for_each(algorithms.begin(), algorithms.end(), [](auto &algorithm) {
            std::cout << algorithm.algorithmName_ << '\n';
        });

        return 0;
    }

    //result settings
    bool to_file = parser.has_argument("-tf") || parser.has_argument("-to_file");
    std::string root_path = parser.get_argument("-path", parser.get_argument("-p", "./results"));

    //precision settings
    uint64_t iterations_special_case = parser.get_ull_argument("-isc", parser.get_ull_argument("-iterations_special_case", DEFAULT_ITERATIONS_SPECIAL_CASE));
    uint64_t iterations_average_case = parser.get_ull_argument("-iac", parser.get_ull_argument("-iterations_average_case", DEFAULT_ITERATIONS_AVERAGE_CASE));

    //enable or disable specific parts of the benchmark
    bool many_different = parser.has_argument("-md") || parser.has_argument("-many_different");
    bool many_equal = parser.has_argument("-me") || parser.has_argument("-many_equal");
    long double percentage = parser.get_ld_argument("-mep", parser.get_ld_argument("-many_equal_percentage", DEFAULT_PERCENTAGE_EQUAL_VALUES));
    if (percentage < 0) percentage = 0;
    else if (percentage > 50) percentage = 50;
    uint64_t log_n_start = parser.get_ull_argument("-lns", parser.get_ull_argument("log_n_start", DEFAULT_LOG_N_START));
    uint64_t log_n_end = parser.get_ull_argument("-lne", parser.get_ull_argument("-log_n_end", DEFAULT_LOG_N_END));
    bool disable_best_case = parser.has_argument("-dbc") || parser.has_argument("-disable_best_case") || iterations_special_case == 0;
    bool disable_worst_case = parser.has_argument("-dwc") || parser.has_argument("-disable_worst_case") || iterations_special_case == 0;
    bool disable_average_case = parser.has_argument("-dac") || parser.has_argument("-disable_average_case") || iterations_average_case == 0;

    //remove all best cases
    if (disable_best_case) {
        for (auto &algorithm: algorithms) {
            erase_if(algorithm.executedCases_, [](const auto &case_) {
                return case_.first.rfind(BEST_CASE, 0) == 0;
            });
        }
    }

    //remove all worst cases
    if (disable_worst_case) {
        for (auto &algorithm: algorithms) {
            erase_if(algorithm.executedCases_, [](const auto &case_) {
                return case_.first.rfind(WORST_CASE, 0) == 0;
            });
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

    bool enable_algorithms = parser.has_argument("-ea") || parser.has_argument("-enable_algorithm");
    bool disable_algorithms = parser.has_argument("-da") || parser.has_argument("-disable_algorithm");

    if (enable_algorithms && disable_algorithms) {
        std::cerr << "Argument -enable_algorithm/-ea cannot be when -disable_algorithm/-da is used!";
        return 0;
    }

    if (enable_algorithms || disable_algorithms) {
        std::vector<std::string> algorithm_names;
        std::function<bool(AlgorithmInformation &information)> contains = [&algorithm_names](auto &information) {
            return std::find(algorithm_names.begin(), algorithm_names.end(), information.algorithmName_) != algorithm_names.end();
        };

        if (enable_algorithms) algorithm_names = split(parser.get_argument("-ea", parser.get_argument("-enable_algorithm", "")), "|");
        else algorithm_names = split(parser.get_argument("-da", parser.get_argument("-disable_algorithm", "")), "|");

        auto new_end = enable_algorithms ?
                       std::remove_if(algorithms.begin(), algorithms.end(), [&contains](auto &information) { return !contains(information); }) :
                       std::remove_if(algorithms.begin(), algorithms.end(), [&contains](auto &information) { return contains(information); });
        algorithms.erase(new_end, algorithms.end());
    }

    //execute add minimum one of them
    if (many_different || many_different == many_equal) {
        std::cout << "-----------------------------------------------\n";
        std::cout << "Benchmark algorithms with many different values\n";
        std::cout << "-----------------------------------------------\n";
        benchmark(algorithms, MAX_VALUE_MANY_DIFFERENT, log_n_start, log_n_end, to_file, root_path + "/many_different");
        std::cout << '\n';
    }
    if (many_equal || many_equal == many_different) {
        std::cout << "-------------------------------------------\n";
        std::cout << "Benchmark algorithms with many equal values\n";
        std::cout << "-------------------------------------------\n";
        benchmark(algorithms, [percentage](uint64_t n) { return MAX_VALUE_MANY_EQUAL(n, percentage); }, log_n_start, log_n_end, to_file, root_path + "/many_equal");
        std::cout << '\n';
    }
}