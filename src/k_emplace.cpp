#include "k_emplace.hpp"
#include "TeaString.hpp"

// Defining static globals (hence the sg_ prefix)
// These are defined here for performance reasons
static int sg_foundIndex;
static int sg_i;
static bool sg_isInString;
static std::string sg_teaEmplaceString;
static std::string sg_teaStringName;

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
    sg_teaEmplaceString.clear();
    for (const char &c : ts.getvalue())
    {
        if (c == '\\')
            sg_teaEmplaceString.append("\\\\");
        else
            sg_teaEmplaceString.push_back(c);
    }
    if (sg_isInString)
        prestatement.replace(sg_foundIndex, sg_teaStringName.size() + 2, sg_teaEmplaceString);
    else
        prestatement.replace(sg_foundIndex, sg_teaStringName.size() + 4, '"' + sg_teaEmplaceString + '"');
}

// Emplaces variables into statement
void kEmplace(std::string &prestatement, teaString_t &teaStrings)
{
    for (const TeaString &ts : teaStrings)
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
