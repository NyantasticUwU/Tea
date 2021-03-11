#include "k_inline.hpp"
#include "runtea.hpp"
#define TEA_INLINE_KW_LEN 7U

// Called when the inline keyword is called in tea
void kInline(const std::string &statement, const int &line, const char *&filename, teaString_t &teaStrings,
    teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &teaSnippets, teaArray_t &teaArrays)
{
    const std::vector<std::string> &&newfile{statement.substr(TEA_INLINE_KW_LEN)};
    int &&newLine{0};
    const std::string &&fn{filename + std::string{'(' + std::to_string(line) + ')'}};
    const char *&&newFilename{fn.c_str()};
    loopTeaStatements(newfile, newLine, newFilename, teaStrings, teaInts, teaFloats, teaSnippets, teaArrays, "");
}
