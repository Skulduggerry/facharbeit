#include "../definitions.h"
#include "../utils.h"
#include "quick_sort_v1.h"
#include "insertion_sort_v1.h"
#include "bubble_sort_v1.h"

int main() {
    std::vector<sort::AlgorithmInformation> algorithm_information{
            {sort::quicksort<sort::Sortable::iterator>,      "quicksort",
                    {
                            {sort::BEST_CASE, sort::quicksort_best_case_generator},
                            {sort::WORST_CASE, sort::quicksort_worst_case_generator}
                    }
            },
            {sort::insertion_sort<sort::Sortable::iterator>, "insertion_sort",
                    {
                            {sort::BEST_CASE, sort::insertion_sort_best_case_generator},
                            {sort::WORST_CASE, sort::insertion_sort_worst_case_generator}
                    }
            },
            {
             sort::bubble_sort<sort::Sortable::iterator>,    "bubble_sort",
                    {
                            {sort::BEST_CASE, sort::bubble_sort_best_case_generator},
                            {sort::WORST_CASE, sort::bubble_sort_worst_case_generator}
                    }
            }
    };

    sort::benchmark(algorithm_information);
    sort::output(algorithm_information);
}