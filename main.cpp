#include "cmd.hpp"
#include "constants.hpp"
#include "error.hpp"
#include "fileIO.hpp"
#include "init.hpp"
#include "parsefile.hpp"
#include "runtea.hpp"

// Defining globals
const char *g_teakeywords[TEA_NUMBER_OF_KEYWORDS]{
    "assign",
    "delete",
    "elif",
    "else",
    "emplace",
    "end",
    "exit",
    "float",
    "if",
    "import",
    "include",
    "int",
    "string",
    "system",
    "while"};

// Main entry point of program
int main(const int argc, const char **argv)
{
    try
    {
        init();
        const char *&filename{getMainFileName(argc, argv)};
        checkFile(filename);
        std::vector<std::string> premainfile;
        parseFile(filename, premainfile);
        const std::vector<std::string> &mainfile{premainfile};
        teaInt_t teaInts{TeaInt{"argc", argc}};
        teaString_t teaStrings;
        fillTeaStringVec(argc, argv, teaStrings);
        runTea(mainfile, filename, &teaStrings, &teaInts);
    }
    catch (...)
    {
        // Exit with code -1
        teaError("An unknown error has occured. Exiting with code -1.", -1);
    }
    return 0;
}
