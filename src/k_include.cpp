#include "fileIO.hpp"
#include "k_include.hpp"
#include "kwstr.hpp"
#include "parsefile.hpp"
#include "runtea.hpp"

// Called when include keyword is called in tea
void kInclude(const std::string &statement, const int &line, const char *&filename)
{
    // newfilenamestr is needed as newfilename would otherwise be a dangling pointer
    const std::string newfilenamestr{getStringLiteral(statement, line, filename, 7)};
    const char *&&newfilename{newfilenamestr.c_str()};
    checkFile(newfilename);
    std::vector<std::string> _newfile;
    const std::vector<std::string> &newfile{parseFile(_newfile, newfilename)};
    runTea(newfile, newfilename);
}
