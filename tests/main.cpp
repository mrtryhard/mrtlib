#include <iostream>

#include "../system/sysutil.hpp"
#include "types/bounded.hpp"
#include "containers/circular_list.hpp"

int main() {
    bool success = mrt::tests::bounded::execute();
    success = success & mrt::tests::circular_list::execute();

    std::cout << "Test result: " << success << std::endl;
    mrt::system::pause();

    return 0;
}