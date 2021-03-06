#include "error.hpp"
#include "evalops.hpp"
#include "k_emplace.hpp"
#include "k_if.hpp"
#include "k_while.hpp"
#include "runtea.hpp"

// Creates a mutable string from the unchanged statement
static inline void createMutableString(std::string &mutableUnchangedStatement,
    const std::string &unchangedStatement, const int &line, const char *&filename,
    const tea::teaString_t &teaStrings, const tea::teaInt_t &teaInts, const tea::teaFloat_t &teaFloats,
    const tea::teaArray_t &teaArrays)
{
    mutableUnchangedStatement = unchangedStatement;
    if (startsWithKeyword(mutableUnchangedStatement, "emplace "))
        kEmplace(mutableUnchangedStatement, line, filename, teaStrings, teaInts, teaFloats, teaArrays);
    evalOps(mutableUnchangedStatement, line, filename);
}

// Called when while keyword is called in tea
void kWhile(const std::vector<std::string> &teafile, const int &teafileSize, const std::string unchangedStatement,
    int &line, const char *&filename, tea::teaString_t &teaStrings, tea::teaInt_t &teaInts,
    tea::teaFloat_t &teaFloats, tea::teaSnippet_t &teaSnippets, tea::teaArray_t &teaArrays,
    const std::string &currentNamespace)
{
    const int whileLine{line};
    std::string mutableUnchangedStatement;
    createMutableString(mutableUnchangedStatement, unchangedStatement, line, filename, teaStrings, teaInts,
        teaFloats, teaArrays);
    while (isIfTrue(mutableUnchangedStatement, line, filename, 5))
    {
        line = whileLine;
        tea::loopTeaStatements(teafile, line, filename, teaStrings, teaInts, teaFloats, teaSnippets, teaArrays,
            currentNamespace);
        createMutableString(mutableUnchangedStatement, unchangedStatement, line, filename, teaStrings, teaInts,
            teaFloats, teaArrays);
    }
    line = whileLine;
    int &&nif{0};
    while (line < teafileSize)
    {
        if (teafile[line] == "end")
        {
            ++line;
            if (!nif--)
                return;
            continue;
        }
        if (isEnteringBlock(teafile[line]))
        {
            ++line;
            ++nif;
            continue;
        }
        ++line;
    }
    teaSyntaxError(line, filename, "While statement never ended (closed).");
}
