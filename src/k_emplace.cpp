#include "error.hpp"
#include "k_emplace.hpp"
#include "runtea.hpp"
#include "stringsupport.hpp"

// Extracting value from array in the form of a string
static inline const std::string &getArrayValue(const TeaArray &teaArray, const std::string &varname,
    const std::size_t &nameEnd, const bool isInString, const int &line, const char *&filename)
{
    static std::size_t s_pos;
    static std::string s_out;
    const std::string &type{teaArray.gettype()};
    const int &&indexPos{[&]() -> const int {
        const std::size_t &&varnameSize{varname.size()};
        if (varname[nameEnd] != '[')
            teaSyntaxError(line, filename, "Opening brace never found.");
        std::string iPosStr;
        for (std::size_t &&i{nameEnd + 1U}; i < varnameSize; ++i)
        {
            if (varname[i] == ']')
            {
                const int &&ret{std::stoi(iPosStr, &s_pos)};
                if (s_pos != iPosStr.size() || std::to_string(ret)[0U] != iPosStr[0U])
                    teaSyntaxError(line, filename, "Invalid array subscript syntax.");
                return ret;
            }
            iPosStr.push_back(varname[i]);
        }
        teaSyntaxError(line, filename, "Closing brace never found.");
        return std::stoi(iPosStr);
    }()};
    if (teaArray.getsize() <= indexPos || indexPos < 0)
        teaSyntaxError(line, filename, "Array index out of range for " + teaArray.getname() + '.');
    if (type == "string")
    {
        std::string preOut{std::any_cast<TeaString>(teaArray.getdata()[indexPos]).getvalue()};
        unformatTeaString(preOut);
        s_out = (isInString) ? (preOut) : ('"' + preOut + '"');
    }
    else if (type == "int")
        s_out = std::to_string(std::any_cast<TeaInt>(teaArray.getdata()[indexPos]).getvalue());
    else if (type == "float")
        s_out = std::to_string(std::any_cast<TeaFloat>(teaArray.getdata()[indexPos]).getvalue());
    else
        teaSyntaxError(line, filename, "Array of invalid type.");
    return s_out;
}

// Fills full array string
static inline const std::string &fillFullArrayString(const TeaArray &teaArray, const bool &isInString,
    const int &line, const char *&filename)
{
    static std::string s_fullArray;
    static std::string s_toFill;
    s_fullArray.clear();
    const std::string &arrt{teaArray.gettype()};
    const std::vector<std::any> &data{teaArray.getdata()};
    if (arrt == "string")
    {
        for (const std::any &element : data)
        {
            s_toFill.clear();
            if (!isInString)
                s_toFill.push_back('"');
            s_toFill.append(std::any_cast<TeaString>(element).getvalue());
            if (!isInString)
                s_toFill.push_back('"');
            s_fullArray.append(s_toFill + ' ');
        }
    }
    else if (arrt == "int")
    {
        for (const std::any &element : data)
        {
            s_toFill.clear();
            s_toFill.append(std::to_string(std::any_cast<TeaInt>(element).getvalue()));
            s_fullArray.append(s_toFill + ' ');
        }
    }
    else if (arrt == "float")
    {
        for (const std::any &element : data)
        {
            s_toFill.clear();
            s_toFill.append(std::to_string(std::any_cast<TeaFloat>(element).getvalue()));
            s_fullArray.append(s_toFill + ' ');
        }
    }
    else
        teaSyntaxError(line, filename, "Invalid array type.");
    s_fullArray.pop_back();
    return s_fullArray;
}

// Emplaces variable into statement
static inline void emplaceVar(std::string &prestatement, std::size_t &statementSize, bool &isVarFound,
    const std::string &varname, const std::size_t &braceOpenPos, const std::size_t &braceClosePos,
    const bool &isInString, const int &line, const char *&filename, const teaString_t &teaStrings,
    const teaInt_t &teaInts, const teaFloat_t &teaFloats, const teaArray_t &teaArrays)
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
    for (const TeaArray &ta : teaArrays)
    {
        if (startsWithKeyword(varname, ta.getname().c_str()))
        {
            if (prestatement[braceClosePos - 1U] != ']')
                break;
            prestatement.replace(
                prestatement.begin() + braceOpenPos,
                prestatement.begin() + braceClosePos + 1U,
                getArrayValue(ta, varname, ta.getname().size(), isInString, line, filename));
            statementSize = prestatement.size();
            isVarFound = true;
            return;
        }
        else if (ta.getname() == varname)
        {
            prestatement.replace(
                prestatement.begin() + braceOpenPos,
                prestatement.begin() + braceClosePos + 1U,
                fillFullArrayString(ta, isInString, line, filename));
            statementSize = prestatement.size();
            isVarFound = true;
            return;
        }
    }
    isVarFound = false;
}

// Emplaces variables into statement
void kEmplace(std::string &prestatement, const int &line, const char *&filename, const teaString_t &teaStrings,
    const teaInt_t &teaInts, const teaFloat_t &teaFloats, const teaArray_t &teaArrays)
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
            s_varname.clear();
            continue;
        }
        if (prestatement[i] == '}' && isInBrace)
        {
            s_braceClosePos = i;
            if (isInBrace)
            {
                emplaceVar(prestatement, statementSize, s_isVarFound, s_varname, s_braceOpenPos, s_braceClosePos,
                    isInString, line, filename, teaStrings, teaInts, teaFloats, teaArrays);
                i = s_isVarFound ? 0U : s_braceClosePos;
                isInString = s_isVarFound ? false : isInString;
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
