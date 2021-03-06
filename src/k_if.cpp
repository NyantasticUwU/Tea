#include "error.hpp"
#include "evalops.hpp"
#include "k_emplace.hpp"
#include "k_if.hpp"
#include "runtea.hpp"

// Checks if tea if statement evaluates to true
const bool isIfTrue(const std::string &statement, const int &line, const char *&filename, const int &kwlen)
{
    static thread_local std::string s_evaluation;
    s_evaluation.clear();
    const std::size_t &&statementSize{statement.size()};
    if (statementSize < static_cast<std::size_t>(kwlen + 2))
        teaSyntaxError(line, filename);
    for (std::size_t &&i{static_cast<std::size_t>(kwlen + 1)}; i < statementSize; ++i)
    {
        if (statement[i] == ' ')
            teaSyntaxError(line, filename);
        s_evaluation.push_back(statement[i]);
    }
    return std::stoi(s_evaluation);
}

// Checks if statement enters a new block
const bool isEnteringBlock(const std::string &statement) noexcept
{
    return startsWithKeyword(statement, "if ") || startsWithKeyword(statement, "while ") ||
        startsWithKeyword(statement, "snippet ") ||
        startsWithKeyword(statement, "emplace if ") || startsWithKeyword(statement, "emplace while ") ||
        startsWithKeyword(statement, "emplace snippet ");
}

// Called when if keyword is called in tea
void kIf(const std::vector<std::string> &teafile, const int &teafileSize, const std::string &statement, int &line,
    const char *&filename, tea::teaString_t &teaStrings, tea::teaInt_t &teaInts, tea::teaFloat_t &teaFloats,
    tea::teaSnippet_t &teaSnippets, tea::teaArray_t &teaArrays, const std::string &currentNamespace)
{
    if (isIfTrue(statement, line, filename, 2))
        tea::loopTeaStatements(teafile, line, filename, teaStrings, teaInts, teaFloats, teaSnippets, teaArrays,
            currentNamespace);
    else
    {
        int &&nif{0};
        while (line < teafileSize)
        {
            std::string nextline{teafile[line]};
            if (startsWithKeyword(nextline, "emplace "))
                kEmplace(nextline, line, filename, teaStrings, teaInts, teaFloats, teaArrays);
            evalOps(nextline, line, filename);
            if (nextline == "end")
            {
                ++line;
                if (!nif--)
                    return;
                continue;
            }
            if (isEnteringBlock(nextline))
            {
                ++line;
                ++nif;
                continue;
            }
            if (startsWithKeyword(nextline, "elif") && !nif)
            {
                ++line;
                kIf(teafile, teafileSize, nextline.substr(2U, nextline.size()), line, filename, teaStrings,
                    teaInts, teaFloats, teaSnippets, teaArrays, currentNamespace);
                return;
            }
            if (nextline == "else" && !nif)
            {
                ++line;
                tea::loopTeaStatements(teafile, line, filename, teaStrings, teaInts, teaFloats, teaSnippets,
                    teaArrays, currentNamespace);
                return;
            }
            ++line;
        }
        teaSyntaxError(line, filename, "If statement never ended (closed).");
    }
}
