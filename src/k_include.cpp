#include "k_include.hpp"
#include "parsefile.hpp"
#include "throwerror.hpp"

// Defining globals
static std::vector<std::string> newParsedFile;

// This function is called whenever the include keyword is used
void kInclude(std::string &statement, std::vector<std::string> &parsedFile, const int16_dynamic_t &index)
{
    if (std::count(statement.begin(), statement.end(), '"') <= 1)
        throwError(statement);
    statement.erase(0, statement.find_first_of('"') + 1);
    statement.erase(statement.find_last_of('"'));
    newParsedFile = parseFile(statement);
    parsedFile.insert(parsedFile.begin() + index, newParsedFile.begin(), newParsedFile.end());
    parsedFile.erase(parsedFile.begin() + index + newParsedFile.size());
}
