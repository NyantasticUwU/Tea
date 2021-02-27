#include "fileIO.hpp"
#include "k_import.hpp"
#include "kwstr.hpp"
#include "parsefile.hpp"
#include "runtea.hpp"

// Called when import keyword is called in tea
void kImport(const std::string &statement, const int &line, const char *&filename, teaString_t &teaStrings,
    teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &teaSnippets, teaArray_t &teaArrays)
{
    // newfilenamestr is needed as newfilename would otherwise be a dangling pointer
    const std::string newfilenamestr{getStringLiteral(statement, line, filename, 6)};
    const char *&&newfilename{newfilenamestr.c_str()};
    checkFile(newfilename);
    std::vector<std::string> _newfile;
    const std::vector<std::string> &newfile{parseFile(_newfile, newfilename)};
    int &&newline{0};
    loopTeaStatements(newfile, newline, newfilename, teaStrings, teaInts, teaFloats, teaSnippets, teaArrays);
}
