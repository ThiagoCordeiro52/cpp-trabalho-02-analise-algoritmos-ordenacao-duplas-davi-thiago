/*!
 * @file main.cpp
 * Benchmarking suit to compare sorting algorithms under verios situations.
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <string>
#include <cassert>
#include <algorithm>
#include <functional>
#include <random>
#include <bits/stdc++.h>
#include <utility>
using std::function;

#include "lib/sorting.h"

//=== ALIASES

/// Data type we are working with.
using value_type = long int;
/// Size type.
using size_type = long int;
/// Alias for duration measure.
using duration_t = std::chrono::duration<double>;


//=== FUNCTION IMPLEMENTATION.

/// Print time different in an humam readable form.
void print_diff( const std::chrono::time_point<std::chrono::steady_clock> &start,
        const std::chrono::time_point<std::chrono::steady_clock> &end ){
    auto diff = end - start;
    // Milliseconds (10^-3)
    std::cout << "\t\t>>> " << std::chrono::duration <double, std::milli> (diff).count() << " ms" << std::endl;
    // Nanoseconds (10^-9)
    std::cout << "\t\t>>> " << std::chrono::duration <double, std::nano> (diff).count() << " ns" << std::endl;
    // Seconds
    auto diff_sec = std::chrono::duration_cast<std::chrono::seconds>(diff);
    std::cout << "\t\t>>> " << diff_sec.count() << " s" << std::endl;
}

/// The running options
struct RunningOpt{
    size_t min_sample_sz{1000};   //!< Default 10^5.
    size_t max_sample_sz{100000}; //!< The max sample size.
    int n_samples{25};            //!< The number of samples to collect.
    short which_algs{1};          //!< Bit code for the chosen algorithms to run.
    short which_scenarios{1};     //!< Bit code for the chosen scenarios to run.

    /// Returns the sample size step, based on the [min,max] sample sizes and # of samples.
    size_type sample_step(void){
        return static_cast<float>(max_sample_sz-min_sample_sz)/(n_samples-1);
    }
};

enum AlgorithmCode {
    START_ALGR,
    BUBBLE,
    INSERTION,
    SELECTION,
    MERGE,
    SHELL,
    END_ALGR,
};

class AlgorithmCollection {
    const std::string algorithms_names[END_ALGR] {
        "Bubble sort",
        "Insertion sort",
        "Selection sort",
        "Merge sort",
        "Shell sort",
    };
    AlgorithmCode curr_algorithm;

    public:
        AlgorithmCollection() {
            curr_algorithm = START_ALGR;
            next();
        }

        std::string to_string() {
            if (has_ended())
                return "";
            return algorithms_names[curr_algorithm - 1];
        }

        bool has_ended() {
            return curr_algorithm >= END_ALGR;
        }

        void next() {
            curr_algorithm = static_cast<AlgorithmCode>(curr_algorithm + 1);
        }

        template <typename RandomIt, typename Compare>
        void call_curr(RandomIt first, RandomIt last, Compare cmp) {
            switch (curr_algorithm) {
                case BUBBLE:
                    sa::bubble(first, last, cmp);
                    break;
                case INSERTION:
                    sa::insertion(first, last, cmp);
                    break;
                case SELECTION:
                    sa::selection(first, last, cmp);
                    break;
                case MERGE:
                    sa::merge(first, last, cmp);
                    break;
                case SHELL:
                    sa::shell(first, last, cmp);
                    break;
                default: break;

            }
        }
};

enum DataCode {
    START_DATA,
    NON_DECREASING,
    NON_INCREASING,
    ALL_RANDOM,
    SORTED_75,
    SORTED_50,
    SORTED_25,
    END_DATA,
};

class DataSet {
    const std::string names[END_DATA] {
        "non_decreasing",
        "non_increasing",
        "all_random",
        "sorted_75",
        "sorted_50",
        "sorted_25",
    };
    std::vector<int> data;
    DataCode curr_dataset;

    public:
        DataSet(const RunningOpt& run_opt) {
            data.reserve(run_opt.max_sample_sz);
            curr_dataset = START_DATA;
            next();
        }

        std::string get_filename() {
            if (has_ended())
                return "";
            return names[curr_dataset - 1];
        }

        void resize(size_t size) {
            data.resize(size);
        }

        bool has_ended() {
            return curr_dataset >= END_DATA;
        }

        void next() {
            curr_dataset = static_cast<DataCode>(curr_dataset + 1);
            std::default_random_engine generator;
            generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
            bool sort_percent {false};
            float percentage;

            switch (curr_dataset) {
                case NON_DECREASING:
                    for (size_t i {0}; i < data.size(); i++) {
                        data[i] = i;
                    }
                    break;
                case NON_INCREASING: 
                    for (size_t i {0}; i < data.size(); i++) {
                        data[i] = data.size() - i - 1;
                    }
                    break;
                case ALL_RANDOM: {
                    std::uniform_int_distribution<int> distribution(0, data.size() - 1);
                    for (size_t i {0}; i < data.size(); i++) {
                        data[i] = distribution(generator);
                    }
                } break;
                case SORTED_75: 
                    sort_percent = true;
                    percentage = 0.25;
                    break;
                case SORTED_50: 
                    sort_percent = true;
                    percentage = 0.5;
                    break;
                case SORTED_25: 
                    sort_percent = true;
                    percentage = 0.75;
                    break;
                default: break;
            }
            if (sort_percent) {
                int I[data.size()];
                for (size_t i {0}; i < data.size(); i++) {
                    data[i] = i;
                    I[i] = i;
                }
                std::shuffle(I, &I[data.size()], generator);
                for (size_t i {0}; i < percentage * data.size(); i += 2) {
                    std::swap(data[I[i]], data[I[i + 1]]);
                }
            }
    }

    std::vector<int>::iterator begin_data() {
        return data.begin();
    }

    std::vector<int>::iterator end_data() {
        return data.end();
    }

};

/// Comparison function for the test experiment.
constexpr bool compare( const int&a, const int &b ){
    return ( a < b );
}

//=== CONSTANT DEFINITIONS.

/// Number of runs we need to calculate the average runtime for a single algorithm.
constexpr short N_RUNS = 5;

//=== The main function, entry point.
int main( int argc, char * argv[] ){
    // Process any command line arguments.
    // TODO: create proper cli for this program.
    // sort_suit [running_options] <nominal_alg_list>
    // $./sort_suite -min 10000 -max 100000 -s 25 quick insertion radix
    RunningOpt run_opt;
    DataSet dataset{run_opt};

    // FOR EACH DATA SCENARIO DO...
    while (not dataset.has_ended()){
        // Collect data in a linear (arithmetic) scale.
        // FOR EACH SAMPLE SIZE DO...
        std::ofstream out_file;
        std::cout << ">>> Starting " << dataset.get_filename() << '\n';
        out_file.open(dataset.get_filename() + ".txt");
        for (auto ns{1}; ns <= run_opt.n_samples; ns++) {
            // FOR EACH SORTING ALGORITHM DO...
            // Select the first sorting algorithm.
            auto size {run_opt.sample_step() * ns};
            dataset.resize(size);
            AlgorithmCollection algorithms{};
            out_file << size << '\t';
            while (not algorithms.has_ended()) {
                // Run each algorithm N_RUN times and calculate a sample avarage for each dependent variable.
                // FOR EACH RUN DO...This is necessary to reduce any measurement noise.
                std::cout << "\t >>> Running " << algorithms.to_string() << " with size " << size << "...\n";
                double elapsed_time_mean = 0;
                for (auto ct_run(0); ct_run < N_RUNS; ++ct_run) {
                    // Reset timer
                    auto start = std::chrono::steady_clock::now();
                    //================================================================================
                    algorithms.call_curr(dataset.begin_data(), dataset.end_data(), compare);
                    //================================================================================
                    auto end = std::chrono::steady_clock::now();
                    // How long did it take?
                    auto diff {end - start};
                    // -------------------------------------------------------------------------------
                    // Calculating a running (repeatedly updated) sample average.
                    // Updating elapsed time sample mean.
                    elapsed_time_mean += (std::chrono::duration <double, std::milli> (diff).count() - elapsed_time_mean) / static_cast<double>(ct_run + 1);
                } // Loop all runs for a single sample size.
                out_file << elapsed_time_mean << '\t';
                algorithms.next();
            } // Loop through all algorithms activated.
            // DATA COLLECTION FOR THIS SAMPLE SIZE (ROW) ENDS HERE.
            // If this is the first time, we must first print the header.
            //  if ( not printed_header ){
            //  }
            // Send out data line to the output file.
            out_file << std::endl;
            // Reset dataline for the next row (sample size).
            // data_line.str("");
        } // Loop through each sample size required.
        // Go to the next active scenario.
        dataset.next();
        // Close the file corresponding to this dataset.
        out_file.close();
    } // Loop data scenarios.

    return EXIT_SUCCESS;
}



