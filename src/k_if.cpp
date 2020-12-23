#include "error.hpp"
#include "k_emplace.hpp"
#include "k_if.hpp"
#include "runtea.hpp"

// Defining static globals (hence the sg_ prefix)
// These are defined here for performance reasons
static int sg_i;
static int sg_statementSize;

// Checks if tea if statement evaluates to true
static const bool isTrue(const std::string &statement, const int &line, const char *&filename)
{
    static std::string s_evaluation;
    s_evaluation.clear();
    sg_statementSize = statement.size();
    if (sg_statementSize < 4)
        teaSyntaxError(line, filename);
    for (sg_i = 3; sg_i < sg_statementSize; ++sg_i)
    {
        if (statement[sg_i] == ' ')
            teaSyntaxError(line, filename);
        s_evaluation.push_back(statement[sg_i]);
    }
    return std::stoi(s_evaluation);
}

// Called when if keyword is called in tea
void kIf(std::vector<std::string> &teafile, const int &teafileSize, const std::string &statement, int &line,
         const char *&filename, teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &teaFloats)
{
    static int s_nif;
    if (isTrue(statement, line, filename))
        loopTeaStatements(teafile, line, filename, teaStrings, teaInts, teaFloats);
    else
    {
        s_nif = 0;
        while (line < teafileSize)
        {
            std::string &nextline{teafile[line]};
            if (startsWithKeyword(nextline, "emplace "))
                kEmplace(nextline, teaStrings, teaInts, teaFloats);
            if (nextline == "end")
            {
                ++line;
                if (!s_nif--)
                    return;
                else
                    continue;
            }
            if (startsWithKeyword(nextline, "if "))
            {
                ++line;
                ++s_nif;
                continue;
            }
            if (startsWithKeyword(nextline, "elif") && !s_nif)
            {
                ++line;
                kIf(teafile, teafileSize, nextline.substr(2, nextline.size()), line, filename, teaStrings, teaInts,
                    teaFloats);
                return;
            }
            if (nextline == "else" && !s_nif)
            {
                ++line;
                loopTeaStatements(teafile, line, filename, teaStrings, teaInts, teaFloats);
                return;
            }
            ++line;
        }
        teaSyntaxError(line, filename, "If statement never ended (closed).");
    }
}
