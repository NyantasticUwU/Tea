#include "error.hpp"
#include "k_exit.hpp"

// Called when tea keyword is called in tea
void kExit(const std::string &statement, const int &line, const char *&filename)
{
    const std::string &&substr{statement.substr(5U)};
    const int &&exitCode{std::stoi(substr)};
    if (std::to_string(exitCode) != substr)
        teaSyntaxError(line, filename, "Invalid exit code.");
    std::exit(exitCode);
}
