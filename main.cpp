#include "cmd.hpp"
#include "error.hpp"
#include "fileIO.hpp"
#include "init.hpp"
#include "parsefile.hpp"
#include "runtea.hpp"

// Main entry point of program
int main(const int argc, const char **argv)
{
    try
    {
        tea::init();
        const char *&filename{getMainFileName(argc, argv)};
        checkFile(filename);
        std::vector<std::string> _mainfile;
        const std::vector<std::string> &mainfile{parseFile(_mainfile, filename)};
        tea::teaInt_t &&teaInts{tea::TeaInt{"argc", argc}};
        tea::teaArray_t teaArrays;
        fillTeaArgs(argc, argv, teaArrays);
        tea::runTea(mainfile, filename, "", nullptr, &teaInts, nullptr, nullptr, &teaArrays);
    }
    catch (...) { teaError("An unknown error has occured. Exiting with code -1.", -1); }
    return 0;
}
