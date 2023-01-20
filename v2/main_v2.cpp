#include "../definitions.h"
#include "../utils.h"
#include "quicksort_v2.h"

int main() {
    std::vector<sort::AlgorithmInformation> algorithm_information{
            {sort::quicksort<sort::Sortable::iterator>,                  "quicksort",
                    {
                            {sort::BEST_CASE,  sort::quicksort_best_case_generator},
                            {sort::WORST_CASE, sort::quicksort_worst_case_generator}
                    }
            },
            {sort::quick_sort_median_of_3<sort::Sortable::iterator>, "quick_sort_median_of_3",
                    {
                            {sort::WORST_CASE, sort::quicksort_worst_case_generator}
                    }
            }
    };

    sort::benchmark(algorithm_information);
    sort::output(algorithm_information);
}