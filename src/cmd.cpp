#include "constants.hpp"
#include "cmd.hpp"
#include "error.hpp"
#include <CLI/CLI.hpp>
#include <iostream>

// Checks if c-string ends with .tea
static const bool isTeaFile(const char *&arg)
{
    for (std::size_t i{0U}; arg[i] != '\0'; ++i)
    {
        if (arg[i] == '.' && arg[i + 1U] == 't' && arg[i + 2U] == 'e' && arg[i + 3U] == 'a' && arg[i + 4U] == '\0')
            return true;
    }
    return false;
}

// Contains all flag/option handler functions
namespace flags
{
    static void versionFlagCallback(const std::int64_t &c)
    {
        std::cout << "Tea version: " << TEA_VERSION << '\n';
    }
}

// Handles command line arguments
static const int handleCMDArgs(const int &argc, const char **&argv)
{
    CLI::App app{"Tea is a personal interpreted programming language.", "Tea"};
    app.add_flag_function("-v,--version", flags::versionFlagCallback, "Outputs version of Tea");
    CLI11_PARSE(app, argc, argv);
    return 0;
}

// Extracts the file name of the .tea file from the argument vector
const char *&getMainFileName(const int &argc, const char **&argv)
{
    if (argc > 1)
    {
        if (isTeaFile(argv[1]))
            return argv[1];
        std::exit(handleCMDArgs(argc, argv));
    }
    std::exit(0);
}

// Fills tea string vector with cmd args
void fillTeaStringVec(const int &argc, const char **&argv, teaString_t &teaStrings)
{
    for (int i{0}; i < argc; ++i)
        teaStrings.push_back(TeaString{"arg" + std::to_string(i), argv[i]});
}
