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
#include <iterator>
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

std::ostream& operator<<( std::ostream& out, const std::vector<int>& vec ) {
    out << "[ ";
    for( auto value : vec )
        out << value << ' ';
    out << "]\n";

    return out;
}

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
    size_t min_sample_sz{1000};  //!< Default 10^5.
    size_t max_sample_sz{10000}; //!< The max sample size.
    int n_samples{25};           //!< The number of samples to collect.

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
    QUICK,
    RADIX,
    END_ALGR,
};

class AlgorithmCollection {
    const std::string algorithms_names[END_ALGR] {
        "bubble",
        "insertion",
        "selection",
        "merge",
        "shell",
        "quick",
        "radix",
    };
    AlgorithmCode curr_algorithm;

    public:
        AlgorithmCollection() {
            curr_algorithm = START_ALGR;
            next();
        }

        void next() {
            curr_algorithm = static_cast<AlgorithmCode>(curr_algorithm + 1);
        }

        bool has_ended() {
            return curr_algorithm >= END_ALGR;
        }

        std::string to_string() {
            if (has_ended())
                return "";
            return algorithms_names[curr_algorithm - 1];
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
                case QUICK:
                    sa::quick(first, last, cmp);
                    break;
                case RADIX:
                    sa::radix(first, last, cmp);
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
    std::vector<int> data;
    DataCode curr_dataset;
    const std::string names[END_DATA] {
        "non_decreasing",
        "non_increasing",
        "all_random",
        "sorted_75",
        "sorted_50",
        "sorted_25",
    };

    public:
        DataSet(const RunningOpt& run_opt) {
            data.reserve(run_opt.max_sample_sz);
            curr_dataset = START_DATA;
            next();
        }

        void next() {
            curr_dataset = static_cast<DataCode>(curr_dataset + 1);
        }

        bool has_ended() {
            return curr_dataset >= END_DATA;
        }

        std::string to_string() {
            if (has_ended())
                return "";
            return names[curr_dataset - 1];
        }

        void resize(size_t size) {
            data.resize(size);
        }

        void generate_data() {
            std::default_random_engine generator;
            generator.seed(std::chrono::system_clock::now().time_since_epoch().count());
            bool sort_percent {false};
            float percentage;

            switch (curr_dataset) {
                case NON_DECREASING: {
                    std::uniform_int_distribution<int> distribution(0, INT_MAX / data.size());
                    data[0] = distribution(generator);
                    for (size_t i {1}; i < data.size(); i++) {
                        data[i] = data[i - 1] + distribution(generator);
                    }
                } break;
                case NON_INCREASING: {
                    std::uniform_int_distribution<int> distribution(0, INT_MAX / data.size());
                    data[0] = INT_MAX - distribution(generator);
                    for (size_t i {1}; i < data.size(); i++) {
                        data[i] = data[i - 1] - distribution(generator);
                    }
                } break;
                case ALL_RANDOM: {
                    std::uniform_int_distribution<int> distribution(0, INT_MAX);
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
                I[0] = 0;
                std::uniform_int_distribution<int> distribution(0, INT_MAX / data.size());
                data[0] = distribution(generator);
                for (size_t i {1}; i < data.size(); i++) {
                    data[i] = data[i - 1] + distribution(generator);
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
    
    std::ofstream errors;
    errors.open("errors.txt");
    errors << "DATATYPE\tSIZE\tALGO" << std::endl;

    // FOR EACH DATA SCENARIO DO...
    while (not dataset.has_ended()){
        // Collect data in a linear (arithmetic) scale.
        // FOR EACH SAMPLE SIZE DO...
        std::ofstream out_file;
        out_file.open(dataset.to_string() + ".txt");
        std::cout << ">>> Starting " << dataset.to_string() << '\n';
        for (auto ns{0}; ns < run_opt.n_samples; ns++) {
            // FOR EACH SORTING ALGORITHM DO...
            // Select the first sorting algorithm.
            if (ns == 0)
                out_file << "# SIZE";
            auto size {run_opt.min_sample_sz + run_opt.sample_step() * ns};
            dataset.resize(size);
            dataset.generate_data();

            std::vector<int> backup;
            backup.resize(size);

            // temp vector to check to correcteness of each algo
            std::vector<int> temp;
            temp.resize(size);
            std::copy(dataset.begin_data(), dataset.end_data(), temp.begin());
            std::sort(temp.begin(), temp.end());

            AlgorithmCollection algorithms{};
            std::ostringstream line;
            line << size;
            std::cout << dataset.to_string() << ":\t>>> Size: " << size << '\n';
            while (not algorithms.has_ended()) {
                // Run each algorithm N_RUN times and calculate a sample avarage for each dependent variable.
                // FOR EACH RUN DO...This is necessary to reduce any measurement noise.
                std::cout << "\t\t>>> Running " << algorithms.to_string() << "...\n";
                double elapsed_time_mean = 0;
                for (auto ct_run(0); ct_run < N_RUNS; ++ct_run) {
                    std::copy(dataset.begin_data(), dataset.end_data(), backup.begin());
                    // Reset timer
                    auto start = std::chrono::steady_clock::now();
                    //================================================================================
                    algorithms.call_curr(backup.begin(), backup.end(), compare);
                    //================================================================================
                    auto end = std::chrono::steady_clock::now();
                    // How long did it take?
                    auto diff {end - start};


                    // -------------------------------------------------------------------------------
                    // Calculating a running (repeatedly updated) sample average.
                    // Updating elapsed time sample mean.
                    elapsed_time_mean += (std::chrono::duration <double, std::milli> (diff).count() - elapsed_time_mean) / static_cast<double>(ct_run + 1);
                } // Loop all runs for a single sample size.
                // Check if the algorithm gava the right result
                if (backup != temp) {
                    errors << dataset.to_string() << '\t' << size << '\t' << algorithms.to_string() << std::endl;
                    errors << "\t\tGiven:    " << std::vector<int>(dataset.begin_data(), dataset.end_data());
                    errors << "\t\tExpected: " << temp;
                    errors << "\t\tReceived: " << backup;
                }
                line << '\t' << std::setw(9) << elapsed_time_mean;
                if (ns == 0)
                    out_file << '\t' << std::setw(9) << algorithms.to_string();
                algorithms.next();
            } // Loop through all algorithms activated.
            // DATA COLLECTION FOR THIS SAMPLE SIZE (ROW) ENDS HERE.
            // If this is the first time, we must first print the header.
            // Send out data line to the output file.
            if (ns == 0)
                out_file << std::endl;
            out_file << line.str() << std::endl;
        } // Loop through each sample size required.
        // Go to the next active scenario.
        dataset.next();
        dataset.generate_data();
        // Close the file corresponding to this dataset.
        out_file.close();
    } // Loop data scenarios.

    errors.close();

    return EXIT_SUCCESS;
}
