#include <iostream>

#include "../system/sysutil.hpp"
#include "types/bounded.hpp"

int main() {
    bool success = mrt::tests::bounded::execute();

	std::cout << "Test result: " << success << std::endl;
	mrt::system::pause();

    return 0;
}