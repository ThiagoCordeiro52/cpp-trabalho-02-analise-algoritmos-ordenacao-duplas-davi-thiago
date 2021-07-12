#include <iostream>
#include <array>

#include "sorting.h"

using namespace sa;

bool cmp( int a, int b ) {
    return a < b;
}

template< std::size_t SIZE >
std::ostream& operator<<( std::ostream& out, const std::array<int, SIZE>& vec ) {
    out << "[ ";
    for( auto value : vec )
        out << value << ' ';
    out << "]\n";

    return out;
}

template< class Sorting >
void test( Sorting algorithm ) {
    std::array<int, 6> arr1 = {1, 2, 3, 4, 5, 6};
    std::array<int, 6> arr2 = {6, 5, 4, 3, 2, 1};
    std::array<int, 6> arr3 = {6, 5, 1, 2, 3, 4};
    std::array<int, 6> arr4 = {1, 5, 2, 4, 3, 6};

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
    test(bubble<int*, std::function<bool(int, int)>>);
    std::cout << "Shell sort: \n"; 
    test(shell<int*, std::function<bool(int, int)>>);
}
