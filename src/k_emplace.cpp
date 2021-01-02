#include "k_emplace.hpp"

// Defining static globals (hence the sg_ prefix)
// These are defined here for performance reasons
static int sg_foundIndex;
static int sg_i;
static std::string sg_teaVarName;

// Emplaces string into statement
static void emplaceString(std::string &prestatement, const TeaString &ts)
{
    static bool s_isInString;
    static std::string s_teaEmplaceString;
    s_isInString = false;
    for (sg_i = 8; sg_i < sg_foundIndex; ++sg_i)
    {
        if (prestatement[sg_i] == '"') // statement[sg_i - 1] will not be a backslash
        {
            s_isInString = s_isInString ? false : true;
            continue;
        }
        if (prestatement[sg_i] == '\\' && (prestatement[sg_i + 1] == '\\' || prestatement[sg_i + 1] == '"'))
            ++sg_i;
    }
    s_teaEmplaceString.clear();
    for (const char &c : ts.getvalue())
    {
        if (c == '\\')
            s_teaEmplaceString.append("\\\\");
        else
            s_teaEmplaceString.push_back(c);
    }
    if (s_isInString)
        prestatement.replace(sg_foundIndex, sg_teaVarName.size() + 2U, s_teaEmplaceString);
    else
        prestatement.replace(sg_foundIndex, sg_teaVarName.size() + 2U, '"' + s_teaEmplaceString + '"');
}

// Emplaces variables into statement
void kEmplace(std::string &prestatement, const teaString_t &teaStrings, const teaInt_t &teaInts,
              const teaFloat_t &teaFloats)
{
    for (const TeaString &ts : teaStrings)
    {
        sg_teaVarName = ts.getname();
        while (true)
        {
            sg_foundIndex = prestatement.find('{' + sg_teaVarName + '}');
            if (sg_foundIndex != prestatement.npos)
                emplaceString(prestatement, ts);
            else
                break;
        }
    }
    for (const TeaInt &ti : teaInts)
    {
        sg_teaVarName = ti.getname();
        while (true)
        {
            sg_foundIndex = prestatement.find('{' + sg_teaVarName + '}');
            if (sg_foundIndex != prestatement.npos)
                prestatement.replace(sg_foundIndex, sg_teaVarName.size() + 2U, std::to_string(ti.getvalue()));
            else
                break;
        }
    }
    for (const TeaFloat &tf : teaFloats)
    {
        sg_teaVarName = tf.getname();
        sg_foundIndex = prestatement.find('{' + sg_teaVarName + '}');
        while (true)
        {
            sg_foundIndex = prestatement.find('{' + sg_teaVarName + '}');
            if (sg_foundIndex != prestatement.npos)
                prestatement.replace(sg_foundIndex, sg_teaVarName.size() + 2U, std::to_string(tf.getvalue()));
            else
                break;
        }
    }
    prestatement.erase(0U, 8U);
}
