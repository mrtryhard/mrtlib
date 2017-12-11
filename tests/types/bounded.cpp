#include <iostream>
#include <stdexcept>
#include "bounded.hpp"
#include "../../types/bounded/bounded.hpp"

using namespace mrt::types::bounded;

namespace {
    bool test_bounded_range_lower() {
        try {
            bounded_range<int, 1, 100> ranged(0);
        }
        catch (std::range_error&) {
            return true;
        }

        std::clog << "test_bounded_range_lower: failed.";

        return false;
    }

    bool test_bounded_range_upper() {
        try {
            bounded_range<int, 1, 100> ranged(101);
        }
        catch (std::range_error&) {
            return true;
        }

        std::clog << "test_bounded_range_upper: failed.";

        return false;
    }

    bool test_bounded_range_equal() {
        try {
            bounded_range<int, 1, 100> ranged(1);
        }
        catch (std::range_error& err) {
            std::clog << "test_bounded_range_equal: failed: " << err.what() << std::endl;
            return false;
        }

        try {
            bounded_range<int, 1, 100> ranged(100);
        }
        catch (std::range_error& err) {
            std::clog << "test_bounded_range_equal: failed: " << err.what() << std::endl;
            return false;
        }

        return true;
    }
}

namespace mrt { namespace tests { namespace bounded {

    bool execute() noexcept {
        bool success{ true };
        success = success & test_bounded_range_equal();
        success = success & test_bounded_range_lower();
        success = success & test_bounded_range_upper();
    
        return success;
    }

} } }