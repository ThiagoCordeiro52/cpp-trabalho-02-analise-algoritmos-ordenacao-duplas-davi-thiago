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

    // FOR EACH DATA SCENARIO DO...
    while(not dataset.has_ended()){
        // Collect data in a linear (arithmetic) scale.
        // FOR EACH SAMPLE SIZE DO...
        for ( auto ns{0} ; ns < run_opt.n_samples ; ++ns ){
            // FOR EACH SORTING ALGORITHM DO...
            // Select the first sorting algorithm.
            while ( not sort_algs.has_ended() ){
                // Run each algorithm N_RUN times and calculate a sample avarage for each dependent variable.
                // FOR EACH RUN DO...This is necessary to reduce any measurement noise.
                for( auto ct_run(0) ; ct_run < N_RUNS ; ++ct_run ) {
                    // Reset timer
                    start = std::chrono::steady_clock::now();
                    //================================================================================
                    sorting(dataset.begin_data(), dataset.end_data(), compare);
                    //================================================================================
                    end = std::chrono::steady_clock::now();
                    // How long did it take?
                    auto diff( end - start );
                    // -------------------------------------------------------------------------------
                    // Calculating a running (repeatedly updated) sample average.
                    // Updating elapsed time sample mean.
                    elapsed_time_mean = elapsed_time_mean + (  diff - elapsed_time_mean ) / static_cast<double>(ct_run+1);
                } // Loop all runs for a single sample size.
            } // Loop through all algorithms activated.
            // DATA COLLECTION FOR THIS SAMPLE SIZE (ROW) ENDS HERE.
            // If this is the first time, we must first print the header.
            if ( not printed_header ){
            }
            // Send out data line to the output file.
            out_file << data_line.str() << '\n';
            // Reset dataline for the next row (sample size).
            data_line.str("");
        } // Loop through each sample size required.
        // Go to the next active scenario.
        dataset.next();
        // Close the file corresponding to this dataset.
        out_file.close();
    } // Loop data scenarios.

    return EXIT_SUCCESS;
}



