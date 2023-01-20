#include "../definitions.h"
#include "../utils.h"
#include "../v1/quicksort_v1.h"
#include "quicksort_v3.h"

int main() {
    std::vector<sort::AlgorithmInformation> algorithm_information{
            {sort::quicksort<sort::Sortable::iterator>,           "quicksort",
                    {
                            {sort::BEST_CASE,  sort::quicksort_best_case_generator},
                            {sort::WORST_CASE, sort::same_number_generator}
                    }
            },
            {sort::three_way_quicksort<sort::Sortable::iterator>, "three_way_quicksort",
                    {
                            {sort::WORST_CASE, sort::same_number_generator}
                    }
            }
    };

    sort::benchmark(algorithm_information);
    sort::output(algorithm_information);
}