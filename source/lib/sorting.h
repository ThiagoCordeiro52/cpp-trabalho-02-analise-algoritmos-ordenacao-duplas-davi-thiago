/**
 * Several types of sorting algorithms that work on a data range.
 * @author 
 * @date July 5th, 2021
 * @file sorting.h
 */

#ifndef SORTING_H
#define SORTING_H

#include <sstream>
using std::ostringstream;
#include <iterator>
using std::next;
using std::ostream_iterator;
using std::iter_swap;
#include <functional>
using std::less;
using std::function;
#include <vector>
using std::vector;
#include <array>
using std::array;
#include <algorithm>
using std::max_element;
using std::for_each;
using std::copy;
#include <cmath>
using std::pow;
#include <string>
using std::string;
using std::to_string;

namespace sa { // sa = sorting algorithms
    /// Prints out the range to a string and returns it to the client.
    template <typename FwrdIt>
    std::string to_string(FwrdIt first, FwrdIt last){
        std::ostringstream oss;
        oss << "[ ";
        while (first!=last)
            oss << *first++ << " ";
        oss << "]";
        return oss.str();
    }

    void countingSort(int * first, int * last, int power, int base, int temp) {
        int t;
        int acumalador {0};
        int counter {0};

        std::vector<int> c;
        c.reserve(base);

        for (auto i = 0; i < base; i++) 
        {
            c[i] = 0;
        }

        for (auto i = first; i != last; i++) {
            c[( *(first + counter) / power) % base]++;
            counter++;
        }

        for(auto j = 0; j < base; j++) {
            t = c[j];
            c[j] = acumalador;
            acumalador += t;
        }

        counter = 0;
        for (auto i = first; i != last; i++) {
            temp[c[( *(first + counter) / power) % base]] = *(first + counter);
            c[(*(first + counter)/power)%base]++;
            counter++;
        }

        for (auto i = temp.begin(); i != temp.end(); i++) {
            *first = *i;
            first++;
        }
    }

    //{{{ RADIX SORT
    /*!
     * This function implements the Radix Sorting Algorithm based on the **less significant digit** (LSD).
     * 
     * @note There is no need for a comparison function to be passed as argument.
     *
     * @param first Pointer/iterator to the beginning of the range we wish to sort.
     * @param last Pointer/iterator to the location just past the last valid value of the range we wish to sort.
     * @tparam FwrdIt A forward iterator to the range we need to sort.
     * @tparam Comparator A Comparator type function tha returns true if first argument is less than the second argument.
     */
    template < typename FwrdIt, typename Comparator, typename value_type=long >
    void radix( FwrdIt first, FwrdIt last, Comparator){
        // [1] Determine how many digits the largest integer in the incoming range has.

        int quantity {1};
        int divisor {1};
        auto max = *first;
        for (auto i = first; i != last; i++) {
            if (*i > max) {
                max = *i;
            }
        }

        for(auto j = first; j != last; j++) {
            quantity++;
        }

       
        std::vector<int> temp;
        temp.reserve(quantity);
        while (max > 0) {
            countingSort(first, last, divisor, 10, temp);
            divisor *= 10;
            max /= 10;
        }

        // [2] Traverse the entire range 'n_digits' times, from the less significant to the most significant (i.e. from left to right).
//         for ( size_t i{0} ; i < n_digits ; ++i ){
            // [a]=== Buckets creation.
            // We create the 10 buckets each iteration so that they start empty every time through.

            // [b]=== Range traversal and values distriburion into buckets.
            // For each value of the range we need to examine i-th less significant digit
            // so that we can assign the value to the corresponding bucket.

            // ==============================================================
            // What the for_each above does is:
            // --------------------------------------------------------------
            // Assuming value=123 and that the largest number in the collection has 4 digits.
            // 1st pass: 123/1 = 123; 123 % 10 = 3. buckets[3].push_back(123).
            // 2nd pass: 123/10 = 12; 12 % 10 = 2.  buckets[2].push_back(123).
            // 3rd pass: 123/100 = 1; 1 % 10 = 1.   buckets[1].push_back(123).
            // 4th pass: 123/1000 = 0; 0 % 10 = 0.  buckets[0].push_back(123).

            // [c]=== Reverse movement from buckets back into range.
            // At the end of the traversal, we copy all values from the buckets
            // back to the original range. They naturally come in ascending order
            // considering the current i-th digit.
                // Points to the location in memory where the values should be copied to.
                // For each bucket...
                    // ... copy the bucket's stored values back into the range, and
            // ...at the same time, update the destination pointer for the next iteration.
//         } // for to traverse the digits.
    }
    //}}} RADIX SORT

    //{{{ INSERTION SORT
    /// Implementation of the Insertion Sort algorithm.
    template< typename RandomIt, typename Compare >
    void insertion(RandomIt first, RandomIt last, Compare cmp){
        for(auto i = first + 1; i != last; i++) 
        {
            auto auxiliaryI = *i;
            auto j = i;
            for (; (j != first) && cmp(auxiliaryI, *(j - 1)); j-- ) 
            {
                 *j = *(j-1);
            }
            *j = auxiliaryI;
        }
    }
    //}}} INSERTION SORT

    //{{{ SELECTION SORT
    template< typename RandomIt, typename Compare >
    void selection(RandomIt first, RandomIt last, Compare cmp){
       for (auto i = first; i != last - 1; ++i) {
        auto smallest = i;
        for (auto j = i + 1; j != last; j++) {
            if (cmp(*j, *smallest)) {
                smallest = j;
            }
        }
        if(i != smallest) {
            std::iter_swap(smallest, i);
        }
       }
    }
    //}}} SELECTION SORT

    //{{{ BUBBLE SORT
    /**
     * @brief Applies bubble sort on the range [first, last)
     *
     * @tparam RandomIt iterator type
     * @tparam Compare type of predicate to compare objects
     * @param first iterator to the beggining of the range to be sorted
     * @param last iterator to the position after the end of the range to be sorted
     * @param cmp predicate that returns true if the first argument is less than the second 
     */
    template< typename RandomIt, typename Compare >
    void bubble(RandomIt first, RandomIt last, Compare cmp){
        for (auto i{ first }; i != last - 1; i++) {
            for (auto j{ i + 1 }; j != last; j++) {
                if (cmp(*j, *i))
                    std::iter_swap(i, j);
            }
        }
    }
    //}}} BUBBLE SORT

    //{{{ SHELL SORT
    /**
     * @brief Applies shell sort on the range [first, last)
     *
     * @tparam RandomIt iterator type
     * @tparam Compare type of predicate to compare objects
     * @param first iterator to the beggining of the range to be sorted
     * @param last iterator to the position after the end of the range to be sorted
     * @param cmp predicate that returns true if the first argument is less than the second 
     */
    template< typename RandomIt, typename Compare >
    void shell(RandomIt first, RandomIt last, Compare cmp){
        auto n {std::distance(first, last)};

        // k will be incremented at each iteraction,
        // and the gap will be 2 * floor(n / 2^(k + 1)) + 1 at each iteraction, until it is 1
        int k{ 1 };
        int gap;
        do {
            gap = 2 * floor(n / pow(2, k + 1)) + 1;

            for (int i {gap}; i < n; i++) {
                // uses insertion sort with a step of gap
                auto aux = first[i];
                int j {i};
                // iterates from i to gap, decreasing at a step of gap
                // and until the value is greater than aux
                while (j >= gap and cmp(aux, first[j - gap])) {
                    first[j] = first[j - gap];
                    j -= gap;
                }
                first[j] = aux;
            }

            k++;
        } while (gap > 1);
    }
    //}}} SHELL SORT

    //{{{ MERGE SORT
    /**
     * @brief Applies merge sort on the range [first, last)
     *
     * @tparam RandomIt iterator type
     * @tparam Compare type of predicate to compare objects
     * @param first iterator to the beggining of the range to be sorted
     * @param last iterator to the position after the end of the range to be sorted
     * @param cmp predicate that returns true if the first argument is less than the second 
     */
    template< typename RandomIt, typename Compare >
    void merge(RandomIt first, RandomIt last, Compare cmp){
        auto size {std::distance(first, last)};
        
        if (size <= 1)
            return;

        auto mid {first + size / 2 };

        // merge sort the two halfs of the array
        merge(first, mid, cmp);
        merge(mid, last, cmp);

        auto size_1 {size / 2};
        auto size_2 {size - size_1};

        // Creates two temp arrays to store the two sorted ranges
        int range_1[size_1];
        int range_2[size_2];

        // Copies the values of the two ranges to the arrays
        std::copy(first, mid, range_1);
        std::copy(mid, last, range_2);

        auto i {0};
        auto j {0};
        // Itereates over the two arrays, putting the values back to the original range in the right order
        while (i < size_1 && j < size_2) {
            if (cmp(range_1[i], range_2[j])) {
                *first = range_1[i];
                i++;
            } else {
                *first = range_2[j];
                j++;
            }
            first++;
        }

        // If the there was any values left in any of the arrays, copies them to the end of the original range
        if (i < size_1)
            std::copy(&range_1[i], &range_1[size_1], first);
        else if (j < size_2)
            std::copy(&range_2[j], &range_2[size_2], first);
    }
    //}}} MERGE SORT

    //{{{ QUICK SORT
    /*!
     * Partition reorders the elements in the range [first;last) in such a way that
     * all elements **less** than the pivot appear before the pivot, and all elements
     * equal or greater than the pivot appear after the pivot.
     *
     * Any relative order among elements of the array might not be preserved.
     *
     * By **less** we mean a _strict order_ binary relation among the elements we
     * wantt to partition.
     *
     * \note We assume the pivot is a valid iterator in [first; last).
     *
     * @param first The first element in the range we want to reorder.
     * @param last Past the last element in the range we want to reorder.
     * @param pivot Location of the pivot element we need to partition the array.
     * @param cmp A comparison function that returns true if the first parameter is **less** than the second.
     * @return An iterator to the new pivot location within the range.
     */
    template<class FwrdIt, class Compare>
    FwrdIt partition(FwrdIt first, FwrdIt last, FwrdIt pivot, Compare cmp){
        // Let us apply the Lamuto's median-of-three pivot selection strategy
        // to avoid segfault (stack overflow) in case the array is already
        // sorted.

        // Slow/fast approach: operating within the own range.

        // Traverse range, rearranging the elements
            // Move smallest to the front region of the array.
            // Advance frontier..

        // We need a final swap, so that the pivot end up in its final position
        // in the sorted array.
    }
    /// Quick sort implementation.
    template<typename RandomIt, typename Compare>
    void quick(RandomIt first,RandomIt last,Compare comp){
        // TODO
    }
    //}}} QUICK SORT
};
#endif // SORTING_H
