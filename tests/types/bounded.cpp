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

    bool test_has_operator_plus() {
        bounded_range<int, 0, 100> test(40);
        auto resultat = test + 10;
        return resultat.value() == 50;
    }

    bool test_has_operator_minus() {
        bounded_range<int, 0, 100> test(40);
        auto resultat = test - 10;
        return resultat.value() == 30;
    }

    bool test_has_operator_divide() {
        bounded_range<int, 0, 100> test(40);
        auto resultat = test / 10;
        return resultat.value() == 4;        
    }

    bool test_has_operator_multiply() {
        bounded_range<int, 0, 100> test(5);
        auto resultat = test * 4;
        return resultat.value() == 20;          
    }

    bool test_has_operator_modulo() {
        bounded_range<int, 0, 100> test(5);
        auto resultat = test % 4;
        return resultat.value() == 1;          
    }
}

namespace mrt { namespace tests { namespace bounded {

    bool execute() noexcept {
        bool success{ true };
        success = success & test_bounded_range_equal();
        success = success & test_bounded_range_lower();
        success = success & test_bounded_range_upper();
        success = success & test_has_operator_plus();
        success = success & test_has_operator_minus();
        success = success & test_has_operator_divide();
        success = success & test_has_operator_multiply();
        success = success & test_has_operator_modulo();

        return success;
    }

} } }