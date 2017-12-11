#ifndef MRT_SYSTEM_SYSUTIL_HPP_
#define MRT_SYSTEM_SYSUTIL_HPP_

#include <iostream>

namespace mrt { namespace system {
    void pause()
    {
        const auto old_flags = std::cin.flags();
        char wait_char;
    
        std::cin.setf(old_flags & ~std::ios_base::skipws);
    
        std::cout << "Press any key to continue..." << std::endl;
        std::cin >> std::noskipws >> wait_char;
    
        std::cin.setf(old_flags);
    }
} }

#endif // MRT_SYSTEM_SYSUTIL_HPP_