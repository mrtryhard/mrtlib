#include <iostream>

#include "bounded.hpp"

using namespace mrt::types::bounded;

int main() {
    bounded_range<unsigned char, 1, 100> scale{ 20 };


    return 0;
}