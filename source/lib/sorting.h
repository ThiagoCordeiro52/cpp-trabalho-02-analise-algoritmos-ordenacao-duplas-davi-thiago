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
        int divider {1};
        int digito {0};
        auto max {*std::max_element(first, last)};

        auto temp {*first};
        using ValueType = decltype(temp);
        vector<vector<ValueType>> temp_array(10);

        // Loop, until gone trough all digits of max
        while (max > 0) {
            int counter{0};
            for(auto i = first; i < last; i++) {
                digito = (*i / divider) % 10;
                temp_array[digito].push_back(*i);
            }

            for(auto i = 0; i < 10; i++) {
                int quantidade = temp_array[i].size();
                for(int j = 0; j < quantidade; j++) {
                    *(first + counter) = temp_array[i][j];
                    counter++;
                }
            }

            for(auto j = 0; j < 10; j++) {
                if(temp_array[j].size() > 0) {
                    temp_array[j].clear();
                }
            }

            // Below update values that are based on base 10
            divider *= 10; 
            max /= 10;
        }

    }
    //}}} RADIX SORT

    //{{{ INSERTION SORT
    /// Implementation of the Insertion Sort algorithm.
    template< typename RandomIt, typename Compare >
    void insertion(RandomIt first, RandomIt last, Compare cmp){
        for (auto i = first + 1; i != last; i++) {
            auto auxiliaryI = *i;
            auto j = i;
            for (; (j != first) && cmp(auxiliaryI, *(j - 1)); j-- ) {
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
            std::iter_swap(smallest, i);
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
        do {
            auto new_last {first};
            for (auto i{ first + 1 }; i != last; i++) {
                if (cmp(*i, *(i - 1))) {
                    std::iter_swap(i - 1, i);
                    new_last = i;
                }
            }
            last = new_last;
        } while (std::distance(first, last) > 1);
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
        int k {1};
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

        // applies merge sort on the two halfs of the array
        merge(first, mid, cmp);
        merge(mid, last, cmp);

        // MERGES THE TWO HALFS OF THE ARRAY, KEEPING THEM SORTED
        auto size_L {size / 2};
        auto size_R {size - size_L};

        auto temp = *first;
        using ValueType = decltype(temp);

        // Creates two temp arrays to store the two sorted ranges
        ValueType* range_L = new ValueType[size_L];
        ValueType* range_R = new ValueType[size_R];

        // Copies the values of the two ranges to the arrays
        std::copy(first, mid, range_L);
        std::copy(mid, last, range_R);

        auto i {0};
        auto j {0};
        // Itereates over the two arrays, putting the values back to the original range in the right order
        while (i < size_L && j < size_R) {
            if (cmp(range_L[i], range_R[j])) {
                *first = range_L[i];
                i++;
            } else {
                *first = range_R[j];
                j++;
            }
            first++;
        }

        // If the there was any values left in any of the arrays, copies them to the end of the original range
        if (i < size_L)
            std::copy(&range_L[i], &range_L[size_L], first);
        else if (j < size_R)
            std::copy(&range_R[j], &range_R[size_R], first);

        delete [] range_L;
        delete [] range_R;
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
    FwrdIt partition(FwrdIt first, FwrdIt last, FwrdIt pivot, Compare cmp) {
        std::iter_swap(pivot, last - 1);
        // Slow/fast approach: operating within the own range.
        auto slow {first};
        auto fast {first};

        // Traverse range, rearranging the elements
        while (fast != last) {
            // Move smallest to the front region of the array.
            if (cmp(*fast, *pivot)) {
                std::iter_swap(fast, slow);
                slow++;
            }
            // Advance frontier..
            fast++;
        }

        // We need a final swap, so that the pivot end up in its final position
        // in the sorted array.
        std::iter_swap(slow, pivot);

        return slow;
    }
    /// Quick sort implementation.
    template<typename RandomIt, typename Compare>
    void quick(RandomIt first, RandomIt last, Compare cmp) {
        auto size {std::distance(first, last)};

        if (size <= 1)
            return;

        // Let us apply the Lamuto's median-of-three pivot selection strategy
        // to avoid segfault (stack overflow) in case the array is already
        // sorted.

        auto mid {first + size / 2 };
        if (cmp(*(last - 1), *first))
            std::iter_swap(last - 1, first);
        if (cmp(*mid, *first))
            std::iter_swap(mid, first);
        if (cmp(*mid, *(last - 1)))
            std::iter_swap(mid, last - 1); // To ensure the median is the last value
        
        auto pivot {partition(first, last, last - 1, cmp)};

        quick(first, pivot, cmp);
        quick(pivot + 1, last, cmp);
    }
    //}}} QUICK SORT
};
#endif // SORTING_H
