#include "error.hpp"
#include "k_call.hpp"
#include "runtea.hpp"
#include "stdlib.hpp"

// Extracts characters until the given character is encountered
static void getCharacters(const std::string &statement, std::string &str, std::size_t &&pos, const char &endchar)
{
    for (const std::size_t &&statementSize{statement.size()}; pos < statementSize; ++pos)
    {
        if (statement[pos] == endchar)
            break;
        str.push_back(statement[pos]);
    }
}

// Called when the call keyword is called in tea
void kCall(const std::string &statement, const int &line, const char *&filename, teaString_t &teaStrings,
    teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &teaSnippets, teaArray_t &teaArrays,
    const std::string &currentNamespace)
{
    static std::string s_callType;
    static std::string s_snippetName;
    s_callType.clear();
    getCharacters(statement, s_callType, 5U, ' ');
    if (s_callType == "snippet")
    {
        s_snippetName.clear();
        getCharacters(statement, s_snippetName, 13U, '\0');
        for (const TeaSnippet &ts : teaSnippets)
        {
            if (ts.getname() == s_snippetName)
            {
                int &&ln{0};
                loopTeaStatements(ts.getbody(), ln, filename, teaStrings, teaInts, teaFloats, teaSnippets,
                    teaArrays, currentNamespace);
                return;
            }
        }
        if (s_snippetName[0U] == 's' && s_snippetName[1U] == 't' && s_snippetName[2U] == 'd')
        {
            const bool &&isStandardSnippetFound{handleStandardSnippet(s_snippetName, line, filename, teaStrings,
                teaInts, teaFloats, teaSnippets, teaArrays, currentNamespace)};
            if (isStandardSnippetFound)
                return;
        }
        teaSyntaxError(line, filename, "Snippet could not be found.");
    }
    teaSyntaxError(line, filename, "Invalid call type.");
}
