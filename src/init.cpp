#include "init.hpp"
#include "terminate.hpp"
#include <cstdlib>

// Initializes tea (doesn't do much as of now)
void init()
{
    std::atexit(terminateTea);
#ifdef _WIN32
    std::system("title Tea");
#endif
}
