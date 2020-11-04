#include "error.hpp"
#include "k_string.hpp"
#include "k_system.hpp"
#include "runtea.hpp"

// Defining static globals (hence the sg_ prefix)
// These are defined here for performance reasons
static int sg_foundIndex;
static int sg_i;
static bool sg_isInString;
static std::string sg_teaStringName;

// Defining tea value vectors
teaString_t g_teaStrings;

// Emplaces string into statement
static void emplaceString(std::string &prestatement, const TeaString &ts)
{
    sg_isInString = false;
    for (sg_i = 8; sg_i < sg_foundIndex; ++sg_i)
    {
        if (prestatement[sg_i] == '"') // statement[sg_i - 1] will not be a backslash
        {
            sg_isInString = sg_isInString ? false : true;
            continue;
        }
        if (prestatement[sg_i] == '\\' && (prestatement[sg_i + 1] == '\\' || prestatement[sg_i + 1] == '"'))
            ++sg_i;
    }
    if (sg_isInString)
        prestatement.replace(sg_foundIndex, sg_teaStringName.size() + 2, ts.getvalue());
    else
        prestatement.replace(sg_foundIndex, sg_teaStringName.size() + 4, '"' + ts.getvalue() + '"');
}

// Emplaces variables in statement
static void emplaceVariables(std::string &prestatement)
{
    for (const TeaString &ts : g_teaStrings)
    {
        sg_teaStringName = ts.getname();
        while (true)
        {
            sg_foundIndex = prestatement.find('{' + sg_teaStringName + '}');
            if (sg_foundIndex != prestatement.npos)
                emplaceString(prestatement, ts);
            else
                break;
        }
    }
    prestatement.erase(0, 8);
}

// Runs tea string vector
void runTea(std::vector<std::string> &teafile)
{
    // Looping through tea file lines
    int line{0};
    for (std::string &prestatement : teafile)
    {
        ++line;
        // Comment encountered
        if (!prestatement.find('#'))
            continue;
        // Emplace keyword called
        else if (!prestatement.find("emplace "))
            emplaceVariables(prestatement);
        const std::string &statement{prestatement};

        // String keyword called
        if (!statement.find("string "))
        {
            kString(statement, line);
            continue;
        }
        // System keyword called
        else if (!statement.find("system "))
        {
            kSystem(statement, line);
            continue;
        }
        // Invalid statement
        else if (statement != "")
            teaSyntaxError(line);
    }
}
