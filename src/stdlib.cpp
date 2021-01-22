#include "constants.hpp"
#include "error.hpp"
#include "stdlib.hpp"
#include "TeaStandardSnippet.hpp"
#include <iostream>

// Gets variable by name from tea type vector
template <typename T>
static const T &getTeaVariable(const std::vector<T> &typevec, const std::string &varname)
{
    for (const T &t : typevec)
    {
        if (t.getname() == varname)
            return t;
    }
    teaError("Unable to find variable.", 13);
}

// Checks if any vars are equal to tsVariableName
template <typename T>
static const bool isVar(const std::vector<T> &typevec, const std::string &varname)
{
    for (const T &t : typevec)
    {
        if (t.getname() == varname)
            return true;
    }
    return false;
}

// Contains all std snippets
namespace stdSnippet
{
    // Tea standard input snippet
    static void input(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &)
    {
        static std::string s_in;
        std::getline(std::cin, s_in);
        teaStrings.push_back({"fsInput", s_in});
    }

    // Tea standard is variable snippet
    static void isVariable(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &teaFloats,
                           teaSnippet_t &teaSnippets)
    {
        const std::string &varname{getTeaVariable(teaStrings, "tsVariableName").getvalue()};
        if (isVar(teaStrings, varname) || isVar(teaInts, varname) ||
            isVar(teaFloats, varname) || isVar(teaSnippets, varname))
            teaInts.push_back(TeaInt{"fsIsVariable", 1});
        else
            teaInts.push_back(TeaInt{"fsIsVariable", 0});
    }

    // Tea standard print snippet
    static void print(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &)
    {
        std::cout << getTeaVariable(teaStrings, "tsPrint").getvalue();
    }

    // Tea standard print line snippet
    static void printLine(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &)
    {
        std::cout << getTeaVariable(teaStrings, "tsPrintLine").getvalue() << '\n';
    }

    // Tea standard to float snippet
    static void toFloat(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &)
    {
        if (isVar(teaInts, "tsToFloat"))
        {
            teaFloats.push_back({"fsToFloat",
                                 static_cast<float>(getTeaVariable(teaInts, "tsToFloat").getvalue())});
            return;
        }
        if (isVar(teaStrings, "tsToFloat"))
        {
            teaFloats.push_back({"fsToFloat", std::stof(getTeaVariable(teaStrings, "tsToFloat").getvalue())});
            return;
        }
        teaError("Unable to convert variable to float.", 13);
    }

    // Tea standard to int snippet
    static void toInt(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &)
    {
        if (isVar(teaFloats, "tsToInt"))
        {
            teaInts.push_back({"fsToInt", static_cast<int>(getTeaVariable(teaFloats, "tsToInt").getvalue())});
            return;
        }
        if (isVar(teaStrings, "tsToInt"))
        {
            teaInts.push_back({"fsToInt", std::stoi(getTeaVariable(teaStrings, "tsToInt").getvalue())});
            return;
        }
        teaError("Unable to convert variable to int.", 13);
    }

    // Tea standard to string snippet
    static void toString(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &)
    {
        if (isVar(teaInts, "tsToString"))
        {
            teaStrings.push_back({"fsToString", std::to_string(getTeaVariable(teaInts, "tsToString").getvalue())});
            return;
        }
        if (isVar(teaFloats, "tsToString"))
        {
            teaStrings.push_back({"fsToString",
                                  std::to_string(getTeaVariable(teaFloats, "tsToString").getvalue())});
            return;
        }
        teaError("Unable to convert variable to string.", 13);
    }
} // namespace stdSnippet

// Defining globals
const TeaStandardSnippet g_teastandardsnippets[TEA_NUMBER_OF_STANDARD_SNIPPETS]{
    {"stdInput", stdSnippet::input},
    {"stdIsVariable", stdSnippet::isVariable},
    {"stdPrint", stdSnippet::print},
    {"stdPrintLine", stdSnippet::printLine},
    {"stdToFloat", stdSnippet::toFloat},
    {"stdToInt", stdSnippet::toInt},
    {"stdToString", stdSnippet::toString}};

// Handles snippet by given name
const bool handleStandardSnippet(const std::string &snippetName, teaString_t &teaStrings, teaInt_t &teaInts,
                                 teaFloat_t &teaFloats, teaSnippet_t &teaSnippets)
{
    for (const TeaStandardSnippet &tss : g_teastandardsnippets)
    {
        if (tss.name == snippetName)
        {
            tss.snippet(teaStrings, teaInts, teaFloats, teaSnippets);
            return true;
        }
    }
    return false;
}
