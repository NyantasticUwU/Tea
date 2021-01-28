#include "error.hpp"
#include "evalops.hpp"
#include "k_emplace.hpp"
#include "k_if.hpp"
#include "k_while.hpp"
#include "runtea.hpp"

// Creates a mutable string from the unchanged statement
static void createMutableString(
    std::string &mutableUnchangedStatement, const std::string &unchangedStatement, const int &line,
    const char *&filename, const teaString_t &teaStrings, const teaInt_t &teaInts, const teaFloat_t &teaFloats)
{
    mutableUnchangedStatement = unchangedStatement;
    if (startsWithKeyword(mutableUnchangedStatement, "emplace "))
        kEmplace(mutableUnchangedStatement, teaStrings, teaInts, teaFloats);
    evalOps(mutableUnchangedStatement, line, filename);
}

// Called when while keyword is called in tea
void kWhile(
    const std::vector<std::string> &teafile, const int &teafileSize, const std::string unchangedStatement,
    int &line, const char *&filename, teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &teaFloats,
    teaSnippet_t &teaSnippets)
{
    const int whileLine{line};
    std::string mutableUnchangedStatement;
    createMutableString(
        mutableUnchangedStatement, unchangedStatement, line, filename, teaStrings, teaInts, teaFloats);
    while (isIfTrue(mutableUnchangedStatement, line, filename, 5))
    {
        line = whileLine;
        loopTeaStatements(teafile, line, filename, teaStrings, teaInts, teaFloats, teaSnippets);
        createMutableString(
            mutableUnchangedStatement, unchangedStatement, line, filename, teaStrings, teaInts, teaFloats);
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
