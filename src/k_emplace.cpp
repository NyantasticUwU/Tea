#include "k_emplace.hpp"

// Resets all static variables to their default values
static void resetVariables(bool &isInBrace, bool &isInString, std::size_t &braceClosePos,
                           std::size_t &braceOpenPos, std::size_t &i, std::string &varname) noexcept
{
    isInBrace = false;
    isInString = false;
    braceClosePos = 0U;
    braceOpenPos = 0U;
    i = 0U;
    varname.clear();
}

// Emplaces variable into statement
static void emplaceVar(std::string &prestatement, std::size_t &statementSize, bool &isVarFound,
                       const std::string &varname, const std::size_t &braceOpenPos,
                       const std::size_t &braceClosePos, const bool &isInString, const teaString_t &teaStrings,
                       const teaInt_t &teaInts, const teaFloat_t &teaFloats)
{
    for (const TeaString &ts : teaStrings)
    {
        if (ts.getname() == varname)
        {
            prestatement.replace(prestatement.begin() + braceOpenPos, prestatement.begin() + braceClosePos + 1U,
                                 isInString ? ts.getvalue() : '"' + ts.getvalue() + '"');
            statementSize = prestatement.size();
            isVarFound = true;
            return;
        }
    }
    for (const TeaInt &ti : teaInts)
    {
        if (ti.getname() == varname)
        {
            prestatement.replace(prestatement.begin() + braceOpenPos, prestatement.begin() + braceClosePos + 1U,
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
            prestatement.replace(prestatement.begin() + braceOpenPos, prestatement.begin() + braceClosePos + 1U,
                                 std::to_string(tf.getvalue()));
            statementSize = prestatement.size();
            isVarFound = true;
            return;
        }
    }
    isVarFound = false;
}

// Emplaces variables into statement
void kEmplace(std::string &prestatement, const teaString_t &teaStrings, const teaInt_t &teaInts,
              const teaFloat_t &teaFloats)
{
    static bool s_isInBrace, s_isInString, s_isVarFound;
    static std::size_t s_braceClosePos, s_braceOpenPos, s_i, s_statementSize;
    static std::string s_varname;
    resetVariables(s_isInBrace, s_isInString, s_braceClosePos, s_braceOpenPos, s_i, s_varname);
    s_statementSize = prestatement.size();
    for (; s_i < s_statementSize; ++s_i)
    {
        if (prestatement[s_i] == '"') // statement[s_i - 1] will not be a backslash
        {
            s_isInString = s_isInString ? false : true;
            continue;
        }
        if (prestatement[s_i] == '\\' && (prestatement[s_i + 1U] == '\\' || prestatement[s_i + 1U] == '"'))
        {
            ++s_i;
            continue;
        }
        if (prestatement[s_i] == '{')
        {
            s_isInBrace = true;
            s_braceOpenPos = s_i;
            continue;
        }
        if (prestatement[s_i] == '}' && s_isInBrace)
        {
            s_braceClosePos = s_i;
            if (s_isInBrace)
            {
                emplaceVar(prestatement, s_statementSize, s_isVarFound, s_varname, s_braceOpenPos, s_braceClosePos,
                           s_isInString, teaStrings, teaInts, teaFloats);
                s_i = s_isVarFound ? (s_braceOpenPos > 0U ? s_braceOpenPos - 1U : 0U) : s_braceClosePos;
            }
            s_isInBrace = false;
            s_varname.clear();
            continue;
        }
        if (s_isInBrace)
            s_varname.push_back(prestatement[s_i]);
    }
    prestatement.erase(0U, 8U);
}
