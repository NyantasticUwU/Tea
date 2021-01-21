#include "constants.hpp"
#include "error.hpp"
#include "evalops.hpp"
#include "k_assign.hpp"
#include "k_call.hpp"
#include "k_delete.hpp"
#include "k_elif.hpp"
#include "k_else.hpp"
#include "k_emplace.hpp"
#include "k_exit.hpp"
#include "k_float.hpp"
#include "k_snippet.hpp"
#include "k_if.hpp"
#include "k_import.hpp"
#include "k_include.hpp"
#include "k_int.hpp"
#include "k_string.hpp"
#include "k_system.hpp"
#include "k_while.hpp"
#include "runtea.hpp"

// Declaring globals
extern const char *g_teakeywords[TEA_NUMBER_OF_KEYWORDS];

// Checks if statement starts with the given keyword
const bool startsWithKeyword(const std::string &statement, const char *const &teaKeyword) noexcept
{
    static std::size_t s_statementSize;
    s_statementSize = statement.size();
    for (std::size_t &&i{0U}; i < s_statementSize; ++i)
    {
        if (teaKeyword[i] == '\0')
            return true;
        if (teaKeyword[i] != statement[i])
            return false;
    }
    return false;
}

// Runs tea string vector
void runTea(const std::vector<std::string> &teafile, const char *&filename, const teaString_t *const &pteaStrings,
            const teaInt_t *const &pteaInts, const teaFloat_t *const &pteaFloats,
            const teaSnippet_t *const &pteaSnippets)
{
    // Defining tea value vectors
    teaFloat_t teaFloats;
    if (pteaFloats)
        teaFloats = *pteaFloats;
    teaInt_t teaInts;
    if (pteaInts)
        teaInts = *pteaInts;
    teaString_t teaStrings;
    if (pteaStrings)
        teaStrings = *pteaStrings;
    teaSnippet_t teaSnippets;
    if (pteaSnippets)
        teaSnippets = *pteaSnippets;
    int &&line{0};
    loopTeaStatements(teafile, line, filename, teaStrings, teaInts, teaFloats, teaSnippets);
}

// Runs tea statement
void loopTeaStatements(const std::vector<std::string> &teafile, int &line, const char *&filename,
                       teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &teaFloats,
                       teaSnippet_t &teaSnippets)
{
    // Looping through tea file lines
    const int &&teafileSize{static_cast<int>(teafile.size())};
    std::string prestatement;
    while (line < teafileSize)
    {
        prestatement = teafile[line];
        ++line;
        // Comment encountered
        if (prestatement[0U] == '#')
            continue;
        // Emplace keyword called
        else if (startsWithKeyword(prestatement, g_teakeywords[TEA_KEYWORD_EMPLACE]))
            kEmplace(prestatement, teaStrings, teaInts, teaFloats);
        evalOps(prestatement, line, filename);
        const std::string &statement{prestatement};

        // Assign keyword called
        if (startsWithKeyword(statement, g_teakeywords[TEA_KEYWORD_ASSIGN]))
        {
            kAssign(statement, line, filename, teaStrings, teaInts, teaFloats);
            continue;
        }
        // Call keyword called
        else if (startsWithKeyword(statement, g_teakeywords[TEA_KEYWORD_CALL]))
        {
            kCall(statement, line, filename, teaStrings, teaInts, teaFloats, teaSnippets);
            continue;
        }
        // Delete keyword called
        else if (startsWithKeyword(statement, g_teakeywords[TEA_KEYWORD_DELETE]))
        {
            kDelete(statement, line, filename, teaStrings, teaInts, teaFloats, teaSnippets);
            continue;
        }
        // Elif keyword called
        else if (startsWithKeyword(statement, g_teakeywords[TEA_KEYWORD_ELIF]))
        {
            if (kElif(line, teafile, teafileSize))
                return;
        }
        // Else keyword called
        else if (statement == g_teakeywords[TEA_KEYWORD_ELSE])
        {
            if (kElse(line, teafile, teafileSize))
                return;
        }
        // End keyword called
        else if (statement == g_teakeywords[TEA_KEYWORD_END])
            return;
        // Exit keyword called
        else if (startsWithKeyword(statement, g_teakeywords[TEA_KEYWORD_EXIT]))
        {
            kExit(statement);
            continue;
        }
        // Float keyword called
        else if (startsWithKeyword(statement, g_teakeywords[TEA_KEYWORD_FLOAT]))
        {
            kFloat(statement, line, filename, teaFloats);
            continue;
        }
        // If keyword called
        else if (startsWithKeyword(statement, g_teakeywords[TEA_KEYWORD_IF]))
        {
            kIf(teafile, teafileSize, statement, line, filename, teaStrings, teaInts, teaFloats, teaSnippets);
            continue;
        }
        // Import keyword called
        else if (startsWithKeyword(statement, g_teakeywords[TEA_KEYWORD_IMPORT]))
        {
            kImport(statement, line, filename, teaStrings, teaInts, teaFloats);
            continue;
        }
        // Include keyword called
        else if (startsWithKeyword(statement, g_teakeywords[TEA_KEYWORD_INCLUDE]))
        {
            kInclude(statement, line, filename);
            continue;
        }
        // Int keyword called
        else if (startsWithKeyword(statement, g_teakeywords[TEA_KEYWORD_INT]))
        {
            kInt(statement, line, filename, teaInts);
            continue;
        }
        // Snippet keyword called
        else if (startsWithKeyword(statement, g_teakeywords[TEA_KEYWORD_SNIPPET]))
        {
            kSnippet(teafile, statement, line, filename, teaSnippets);
            continue;
        }
        // String keyword called
        else if (startsWithKeyword(statement, g_teakeywords[TEA_KEYWORD_STRING]))
        {
            kString(statement, line, filename, teaStrings);
            continue;
        }
        // System keyword called
        else if (startsWithKeyword(statement, g_teakeywords[TEA_KEYWORD_SYSTEM]))
        {
            kSystem(statement, line, filename);
            continue;
        }
        // While keyword called
        else if (startsWithKeyword(statement, g_teakeywords[TEA_KEYWORD_WHILE]))
        {
            kWhile(teafile, teafileSize, teafile[line - 1], line, filename, teaStrings, teaInts, teaFloats,
                   teaSnippets);
            continue;
        }
        // Invalid statement
        else if (statement != "")
            teaSyntaxError(line, filename);
    }
}
