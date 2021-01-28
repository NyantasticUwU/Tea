#include "k_emplace.hpp"
#include "stringsupport.hpp"

// Emplaces variable into statement
static void emplaceVar(
    std::string &prestatement, std::size_t &statementSize, bool &isVarFound, const std::string &varname,
    const std::size_t &braceOpenPos, const std::size_t &braceClosePos, const bool &isInString,
    const teaString_t &teaStrings, const teaInt_t &teaInts, const teaFloat_t &teaFloats)
{
    for (const TeaString &ts : teaStrings)
    {
        if (ts.getname() == varname)
        {
            static std::string s_str;
            s_str = ts.getvalue();
            unformatTeaString(s_str);
            prestatement.replace(
                prestatement.begin() + braceOpenPos,
                prestatement.begin() + braceClosePos + 1U,
                isInString ? s_str : '"' + s_str + '"');
            statementSize = prestatement.size();
            isVarFound = true;
            return;
        }
    }
    for (const TeaInt &ti : teaInts)
    {
        if (ti.getname() == varname)
        {
            prestatement.replace(
                prestatement.begin() + braceOpenPos,
                prestatement.begin() + braceClosePos + 1U,
                std::to_string(ti.getvalue()));
            statementSize = prestatement.size();
            isVarFound = true;
            return;
        }
    }
    for (const TeaFloat &tf : teaFloats)
    {
        if (tf.getname() == varname)
        {
            prestatement.replace(
                prestatement.begin() + braceOpenPos,
                prestatement.begin() + braceClosePos + 1U,
                std::to_string(tf.getvalue()));
            statementSize = prestatement.size();
            isVarFound = true;
            return;
        }
    }
    isVarFound = false;
}

// Emplaces variables into statement
void kEmplace(
    std::string &prestatement, const teaString_t &teaStrings, const teaInt_t &teaInts, const teaFloat_t &teaFloats)
{
    static bool s_isVarFound;
    static std::size_t s_braceClosePos, s_braceOpenPos;
    static std::string s_varname;
    s_varname.clear();
    bool &&isInBrace{false};
    bool &&isInString{false};
    std::size_t &&i{0U};
    std::size_t &&statementSize{prestatement.size()};
    for (; i < statementSize; ++i)
    {
        if (prestatement[i] == '"') // statement[i - 1] will not be a backslash
        {
            isInString = isInString ? false : true;
            continue;
        }
        if (prestatement[i] == '\\' && (prestatement[i + 1U] == '\\' || prestatement[i + 1U] == '"'))
        {
            ++i;
            continue;
        }
        if (prestatement[i] == '{')
        {
            isInBrace = true;
            s_braceOpenPos = i;
            continue;
        }
        if (prestatement[i] == '}' && isInBrace)
        {
            s_braceClosePos = i;
            if (isInBrace)
            {
                emplaceVar(
                    prestatement, statementSize, s_isVarFound, s_varname, s_braceOpenPos, s_braceClosePos,
                    isInString, teaStrings, teaInts, teaFloats);
                i = s_isVarFound ? (s_braceOpenPos > 0U ? s_braceOpenPos - 1U : 0U) : s_braceClosePos;
            }
            isInBrace = false;
            s_varname.clear();
            continue;
        }
        if (isInBrace)
            s_varname.push_back(prestatement[i]);
    }
    prestatement.erase(0U, 8U);
}
