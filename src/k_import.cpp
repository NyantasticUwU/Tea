#include "fileIO.hpp"
#include "k_import.hpp"
#include "kwstr.hpp"
#include "parsefile.hpp"
#include "runtea.hpp"

// Called when import keyword is called in tea
void kImport(
    const std::string &statement, const int &line, const char *&filename, const teaString_t &teaStrings,
    const teaInt_t &teaInts, const teaFloat_t &teaFloats)
{
    // newfilenamestr is needed as newfilename would otherwise be a dangling pointer
    const std::string newfilenamestr{getStringLiteral(statement, line, filename, 6)};
    const char *&&newfilename{newfilenamestr.c_str()};
    checkFile(newfilename);
    const std::vector<std::string> &&newfile{parseFile(newfilename)};
    runTea(newfile, newfilename, &teaStrings, &teaInts, &teaFloats);
}
