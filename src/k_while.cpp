#include "error.hpp"
#include "evalops.hpp"
#include "k_emplace.hpp"
#include "k_if.hpp"
#include "k_while.hpp"
#include "runtea.hpp"

// Called when while keyword is called in tea
void kWhile(std::vector<std::string> &teafile, const int &teafileSize, const std::string &statement,
            const std::string unchangedStatement, int &line, const char *&filename, teaString_t &teaStrings,
            teaInt_t &teaInts, teaFloat_t &teaFloats)
{
    static int s_nif;
    const int whileLine{line};
    std::string mutableUnchangedStatement{unchangedStatement};
    if (startsWithKeyword(mutableUnchangedStatement, "emplace "))
        kEmplace(mutableUnchangedStatement, teaStrings, teaInts, teaFloats);
    evalOps(mutableUnchangedStatement, line, filename);
    while (isIfTrue(mutableUnchangedStatement, line, filename, 5))
    {
        line = whileLine;
        loopTeaStatements(teafile, line, filename, teaStrings, teaInts, teaFloats);
        mutableUnchangedStatement = unchangedStatement;
        if (startsWithKeyword(mutableUnchangedStatement, "emplace "))
            kEmplace(mutableUnchangedStatement, teaStrings, teaInts, teaFloats);
        evalOps(mutableUnchangedStatement, line, filename);
    }
    s_nif = 0;
    line = whileLine;
    while (line < teafileSize)
    {
        if (teafile[line] == "end")
        {
            ++line;
            if (!s_nif--)
                return;
            else
                continue;
        }
        if (startsWithKeyword(teafile[line], "if ") || startsWithKeyword(teafile[line], "while ") ||
            startsWithKeyword(teafile[line], "emplace if ") || startsWithKeyword(teafile[line], "emplace while "))
        {
            ++s_nif;
            ++line;
            continue;
        }
        ++line;
    }
    teaSyntaxError(line, filename, "While statement never ended (closed).");
}
