#include "error.hpp"
#include "k_call.hpp"
#include "runtea.hpp"

// Extracts characters until the given character is encountered
static void getCharacters(const std::string &statement, std::string &str, std::size_t &&pos, const char &endchar)
{
    static std::size_t s_statementSize;
    s_statementSize = statement.size();
    for (; pos < s_statementSize; ++pos)
    {
        if (statement[pos] == endchar)
            break;
        str.push_back(statement[pos]);
    }
}

// Called when the call keyword is called in tea
void kCall(const std::string &statement, const int &line, const char *&filename, const teaString_t &teaStrings,
           const teaInt_t &teaInts, const teaFloat_t &teaFloats, const teaSnippet_t &teaSnippets)
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
                runTea(ts.getbody(), filename, &teaStrings, &teaInts, &teaFloats, &teaSnippets);
                return;
            }
        }
        teaSyntaxError(line, filename, "Snippet could not be found.");
    }
    teaSyntaxError(line, filename, "Invalid call type.");
}
