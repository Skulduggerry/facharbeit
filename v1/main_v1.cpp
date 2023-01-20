#include "../definitions.h"
#include "../utils.h"
#include "quicksort_v1.h"
#include "heapsort_v1.h"
#include "insertion_sort_v1.h"
#include "counting_sort_v1.h"

int main() {
    std::vector<sort::AlgorithmInformation> algorithm_information{
            {sort::quicksort<sort::Sortable::iterator>,                "quicksort",
                    {
                            {sort::BEST_CASE, sort::quicksort_best_case_generator},
                            {sort::WORST_CASE, sort::quicksort_worst_case_generator}
                    }
            },
            {sort::heapsort<sort::Sortable::iterator>,                 "heapsort",
                    {}
            },
            {sort::insertion_sort<sort::Sortable::iterator>,           "insertion_sort",
                    {
                            {sort::BEST_CASE, sort::insertion_sort_best_case_generator},
                            {sort::WORST_CASE, sort::insertion_sort_worst_case_generator}
                    }
            },
            {
             sort::counting_sort<sort::Sortable::iterator, MAX_VALUE>, "counting_sort",
                    {}
            }
    };

    sort::benchmark(algorithm_information);
    sort::output(algorithm_information);
}