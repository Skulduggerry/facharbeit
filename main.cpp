#include "definitions.h"
#include "quick_sort.h"
#include "insertion_sort.h"
#include "bubble_sort.h"
#include "utils.h"

std::vector<sort::AlgorithmInformation> algorithm_information{
        {sort::quicksort<sort::Sortable::iterator>,                  "quicksort",
                {
                        {sort::BEST_CASE,  sort::quicksort_best_case_generator},
                        {sort::WORST_CASE, sort::quicksort_worst_case_generator}
                }
        },
        {sort::quicksort_iterative<sort::Sortable::iterator>,        "quicksort_iterative",
                {
                        {sort::BEST_CASE,  sort::quicksort_best_case_generator},
                        {sort::WORST_CASE, sort::quicksort_worst_case_generator}
                }
        },
        {sort::quicksort_random_partition<sort::Sortable::iterator>, "quicksort_random_partition",
                {
                        {sort::WORST_CASE, sort::quicksort_worst_case_generator}
                }
        },
        {sort::insertion_sort<sort::Sortable::iterator>,             "insertion_sort",
                {
                        {sort::BEST_CASE,  sort::insertion_sort_best_case_generator},
                        {sort::WORST_CASE, sort::insertion_sort_worst_case_generator}
                }
        },
        {
         sort::bubble_sort<sort::Sortable::iterator>,                "bubble_sort",
                {
                        {sort::BEST_CASE,  sort::bubble_sort_best_case_generator},
                        {sort::WORST_CASE, sort::bubble_sort_worst_case_generator}
                }
        }
};

int main() {
    using namespace sort;

    for (auto &[algorithm, name, special_cases, per_case_size_results]: algorithm_information) {
        for (auto &[case_, generator]: special_cases) {
            ExecutionResults special_case_result = evaluate(generator, algorithm, name);

            add_results(per_case_size_results, special_case_result, case_);
        }

        ExecutionResults average_case_results = evaluate_average_case(algorithm, name);
        add_results(per_case_size_results, average_case_results, AVERAGE_CASE);
    }

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