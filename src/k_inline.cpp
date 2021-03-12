#include "k_inline.hpp"
#include "runtea.hpp"
#define TEA_INLINE_KW_LEN 7U

// Called when the inline keyword is called in tea
void kInline(const std::string &statement, const int &line, const char *&filename, tea::teaString_t &teaStrings,
    tea::teaInt_t &teaInts, tea::teaFloat_t &teaFloats, tea::teaSnippet_t &teaSnippets, tea::teaArray_t &teaArrays)
{
    const std::vector<std::string> &&newfile{statement.substr(TEA_INLINE_KW_LEN)};
    int &&newLine{0};
    const std::string &&fn{filename + std::string{'(' + std::to_string(line) + ')'}};
    const char *&&newFilename{fn.c_str()};
    tea::loopTeaStatements(newfile, newLine, newFilename, teaStrings, teaInts, teaFloats, teaSnippets, teaArrays,
        "");
}
