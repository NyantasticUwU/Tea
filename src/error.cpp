#include "error.hpp"
#include "spdlog/spdlog.h"

// Outputs error message and exits program
void teaError(const std::string &msg, const int &exitCode)
{
    spdlog::error(msg);
    std::exit(exitCode);
}

// Throws ERROR_INVALID_DATA error with custom message
void teaSyntaxError(const int &line, const char *&filename, const std::string &msg)
{
    spdlog::error("ERROR: Invalid statement on line {} ({}).\n" +
                      msg + (msg == "" ? "" : "\n") +
                      "Exiting with code 13 (ERROR_INVALID_DATA).",
                  line, filename);
    std::exit(13); // Windows' ERROR_INVALID_DATA (13)
}
