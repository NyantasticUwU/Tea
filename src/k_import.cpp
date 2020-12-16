#include "fileIO.hpp"
#include "k_import.hpp"
#include "kwstr.hpp"
#include "parsefile.hpp"
#include "runtea.hpp"

// Called when import keyword is called in tea
void kImport(const std::string &statement, const int &line, const char *&filename, const teaFloat_t &teaFloats,
             const teaInt_t &teaInts, const teaString_t &teaStrings)
{
    // newfilenamestr is needed as newfilename would otherwise be a dangling pointer
    const std::string newfilenamestr{getStringLiteral(statement, line, filename, 6)};
    const char *newfilename{newfilenamestr.c_str()};
    checkFile(newfilename);
    std::vector<std::string> newfile;
    parseFile(newfilename, newfile);
    runTea(newfile, newfilename, &teaStrings, &teaInts, &teaFloats);
}
