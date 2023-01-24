#include <vector>
#include "quicksort.h"
#include "counting_sort.h"
#include "heapsort.h"
#include "insertion_sort.h"
#include "utils.h"

int main() {
    using namespace sort;
    std::vector<AlgorithmInformation> algorithms = {
            {
                    quicksort<Sortable::iterator>,              "quicksort",
                    {
                            {BEST_CASE, quicksort_best_case_generator},
                            {WORST_CASE, quicksort_worst_case_generator}
                    }
            },
            {
                    quick_sort_median_of_3<Sortable::iterator>, "quick_sort_median_of_3",
                    {
                            {BEST_CASE, quicksort_worst_case_generator},
                            {WORST_CASE, same_number_generator}
                    }
            },
            {
                    three_way_quicksort<Sortable::iterator>,    "three_way_quicksort",
                    {
                            {BEST_CASE, same_number_generator},
                            {WORST_CASE, quicksort_worst_case_generator}
                    }
            },
            {
                    counting_sort<Sortable::iterator>,          "counting_sort",
                    {}
            },
            {
                    heapsort<Sortable::iterator>,               "heapsort",
                    {
                            {BEST_CASE, heapsort_best_case_generator}
                    }
            },
            {
                    insertion_sort<Sortable::iterator>,         "insertion_sort",
                    {
                            {BEST_CASE, insertion_sort_best_case_generator},
                            {WORST_CASE, insertion_sort_worst_case_generator}
                    }
            }
    };

    benchmark(algorithms, MAX_VALUE_MANY_DIFFERENT, DEFAULT_LOG_N_START, DEFAULT_LOG_N_END, false);
}