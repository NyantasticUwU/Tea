#include "k_include.hpp"
#include "kwstr.hpp"
#include "parsefile.hpp"
#include "runtea.hpp"

// Called when include keyword is called in tea
void kInclude(const std::string &statement, const int &line, const char *&filename)
{
    const char *newfilename{getStringLiteral(statement, line, filename, 7).c_str()};
    std::vector<std::string> newfile;
    parseFile(newfilename, newfile);
    runTea(newfile, newfilename);
}
