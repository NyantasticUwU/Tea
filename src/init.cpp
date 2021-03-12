#include "init.hpp"
#include "terminate.hpp"
#include <cstdlib>

// Contains all necessities for the Tea C++ API
namespace tea
{
    // Initializes tea
    void init()
    {
        std::atexit(terminateTea);
#ifdef _WIN32
        std::system("title Tea");
#endif
    }
}
