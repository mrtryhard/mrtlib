#include <iostream>

#include "tests/types/bounded.hpp"
#include "system/sysutil.hpp"

int main() {
    bool success = mrt::tests::bounded::execute();

	std::cout << "Test result: " << success << std::endl;
	mrt::system::pause();

    return 0;
}