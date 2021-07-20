#include <iostream>
#include <array>

#include "sorting.h"

using namespace sa;
using value_type = int;

bool cmp( value_type a, value_type b ) {
    return a < b;
}

template< std::size_t SIZE >
std::ostream& operator<<( std::ostream& out, const std::array<value_type, SIZE>& vec ) {
    out << "[ ";
    for( auto value : vec )
        out << value << ' ';
    out << "]\n";

    return out;
}

template< class Sorting >
void test( Sorting algorithm ) {
    std::array<value_type, 6> arr1 = {1, 25, 372, 4871, 59600, 61085};
    std::array<value_type, 10> arr2 = {1886418047, 1465952862, 1342161192, 1150338130, 1089955954, 932730323, 920169337, 797929146, 375143163, 97639692};
    std::array<value_type, 6> arr3 = {61085, 59600, 1, 25, 372, 4871};
    std::array<value_type, 6> arr4 = {1, 59600, 25, 4871, 372, 61085};

    std::cout << "\tBefore sort: \n"; 
    std::cout << "\t\tArr1: " << arr1;
    std::cout << "\t\tArr2: " << arr2;
    std::cout << "\t\tArr3: " << arr3;
    std::cout << "\t\tArr4: " << arr4;

    algorithm(arr1.begin(), arr1.end(), cmp);
    algorithm(arr2.begin(), arr2.end(), cmp);
    algorithm(arr3.begin(), arr3.end(), cmp);
    algorithm(arr4.begin(), arr4.end(), cmp);

    std::cout << "\n\tAfter sort: \n"; 
    std::cout << "\t\tArr1: " << arr1;
    std::cout << "\t\tArr2: " << arr2;
    std::cout << "\t\tArr3: " << arr3;
    std::cout << "\t\tArr4: " << arr4;
}

int main( int argc, char *argv[] ) {
    std::cout << "Bubble sort: \n"; 
    test(bubble<value_type*, std::function<bool(value_type, value_type)>>);
    std::cout << "Shell sort: \n"; 
    test(shell<value_type*, std::function<bool(value_type, value_type)>>);
    std::cout << "Merge sort: \n"; 
    test(merge<value_type*, std::function<bool(value_type, value_type)>>);
    std::cout << "Insertion sort: \n"; 
    test(insertion<value_type*, std::function<bool(value_type, value_type)>>);
    std::cout << "Select sort: \n"; 
    test(selection<value_type*, std::function<bool(value_type, value_type)>>);
    std::cout << "Radix sort: \n"; 
    test(radix<value_type*, std::function<bool(value_type, value_type)>>);
    std::cout << "Quicksort: \n"; 
    test(quick<value_type*, std::function<bool(value_type, value_type)>>);
}
