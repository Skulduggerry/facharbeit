#include "../definitions.h"
#include "../utils.h"
#include "quick_sort_v4.h"

int main() {
    std::vector<sort::AlgorithmInformation> algorithm_information{
            {sort::quicksort<sort::Sortable::iterator>,       "quicksort",
                    {
                            {sort::BEST_CASE, sort::quicksort_best_case_generator},
                            {sort::WORST_CASE, sort::quicksort_worst_case_generator}
                    }
            },
            {sort::tail_quick_sort<sort::Sortable::iterator>, "tail_quick_sort",
                    {
                            {sort::BEST_CASE, sort::quicksort_best_case_generator},
                            {sort::WORST_CASE, sort::quicksort_worst_case_generator}
                    }
            }
    };

    sort::benchmark(algorithm_information);
    sort::output(algorithm_information);
}