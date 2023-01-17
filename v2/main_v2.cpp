#include "../definitions.h"
#include "../utils.h"
#include "quick_sort_v2.h"

int main() {
    std::vector<sort::AlgorithmInformation> algorithm_information{
            {sort::quicksort<sort::Sortable::iterator>,           "quicksort",
                    {
                            {sort::BEST_CASE, sort::quicksort_best_case_generator},
                            {sort::WORST_CASE, sort::quicksort_worst_case_generator}
                    }
            },
            {sort::quicksort_iterative<sort::Sortable::iterator>, "quicksort_iterative",
                    {
                            {sort::BEST_CASE, sort::quicksort_best_case_generator},
                            {sort::WORST_CASE, sort::quicksort_worst_case_generator}
                    }
            },
    };

    sort::benchmark(algorithm_information);
    sort::output(algorithm_information);
}