#include "init.hpp"
#include <cstdlib>

// Initializes tea (doesn't do much as of now)
void init()
{
#ifdef _WIN32
    std::system("title Tea");
#endif
}
