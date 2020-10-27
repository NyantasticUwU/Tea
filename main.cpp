#include "cmd.hpp"
#include "fileIO.hpp"

// Main entry point of program
int main(const int argc, const char **argv)
{
    const char *&filename{getMainFileName(argc, argv)};
    checkMainFile(filename);
    return 0;
}
