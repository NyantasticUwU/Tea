#include "createvar.hpp"
#include "error.hpp"
#include "k_array.hpp"
#include "stringsupport.hpp"
#include "TeaFloat.hpp"
#include "TeaInt.hpp"
#include "TeaString.hpp"

// Defining static globals
static thread_local std::size_t sg_i;
static thread_local std::string sg_element;
static thread_local std::vector<std::any> sg_data;

// Declaring external globals
extern thread_local std::string g_varname;

// Gets array type
static const std::string &getArrayType(const std::string &statement, const std::size_t &statementSize,
    const int &line, const char *&filename)
{
    static thread_local std::string s_type;
    s_type.clear();
    for (sg_i = 6U; sg_i < statementSize; ++sg_i)
    {
        if (statement[sg_i] == '[')
            return s_type;
        s_type.push_back(statement[sg_i]);
    }
    teaSyntaxError(line, filename, "Unable to find open brace for array definition.");
    return s_type;
}

// Gets array size
static const int getArraySize(const std::string &statement, const std::size_t &statementSize, const int &line,
    const char *&filename)
{
    static thread_local std::size_t s_pos;
    static thread_local std::string s_sizestr;
    s_sizestr.clear();
    for (++sg_i; sg_i < statementSize; ++sg_i)
    {
        if (statement[sg_i] == ']')
        {
            const int &&ret{std::stoi(s_sizestr, &s_pos)};
            if (s_pos != s_sizestr.size() || std::to_string(ret)[0U] != s_sizestr[0U])
                teaSyntaxError(line, filename, "Invalid array subscript syntax.");
            return ret;
        }
        s_sizestr.push_back(statement[sg_i]);
    }
    teaSyntaxError(line, filename, "Unable to find close brace for array definition.");
    return -1;
}

// Gets pre array name
static const std::string &getPreArrayName(const std::string &statement, const std::size_t &statementSize,
    const int &line, const char *&filename)
{
    static thread_local std::string s_prename;
    s_prename.clear();
    bool &&isSpaceFound{false};
    for (++sg_i; sg_i < statementSize; ++sg_i)
    {
        if (statement[sg_i] == ' ')
        {
            if (isSpaceFound)
            {
                s_prename.push_back(statement[sg_i]);
                return s_prename;
            }
            isSpaceFound = true;
        }
        s_prename.push_back(statement[sg_i]);
    }
    teaSyntaxError(line, filename, "Invalid array name.");
    return s_prename;
}

// Makes sure data size and array size are the same length
static void assertArraySize(const std::size_t &dataSize, const int &size, const int &line, const char *&filename)
{
    if (static_cast<int>(dataSize) != size)
        teaSyntaxError(line, filename, "Array size does not match number of elements.");
}

// Extracts string data
// std::any is meant to be replaced with TeaString
static const std::vector<std::any> getStringData(const std::string &statement, const std::size_t &statementSize,
    const int &line, const char *&filename)
{
    sg_element.clear();
    sg_data.clear();
    bool &&isStartQT{true};
    if (statement[++sg_i] != '"')
        teaSyntaxError(line, filename, "Invalid array definition.");
    for (; sg_i < statementSize; ++sg_i)
    {
        if (statement[sg_i] == '\\' && (statement[sg_i + 1U] == '\\' || statement[sg_i + 1U] == '"'))
        {
            sg_element.push_back(statement[sg_i]);
            sg_element.push_back(statement[++sg_i]);
            continue;
        }
        if (statement[sg_i] == '"')
        {
            if (isStartQT)
            {
                isStartQT = false;
                continue;
            }
            isStartQT = true;
            formatTeaString(sg_element);
            sg_data.push_back(std::make_any<tea::TeaString>("", sg_element));
            sg_element.clear();
            if (sg_i != statementSize - 1U && (statement[sg_i + 1U] != ' ' || statement[sg_i + 2U] != '"'))
                teaSyntaxError(line, filename, "Invalid array definition.");
            ++sg_i;
            continue;
        }
        sg_element.push_back(statement[sg_i]);
    }
    return sg_data;
}

// Extracts int data
// std::any is meant to be replaced with TeaInt
static const std::vector<std::any> getIntData(const std::string &statement, const std::size_t &statementSize,
    const int &line, const char *&filename)
{
    sg_element.clear();
    sg_data.clear();
    if (statement[++sg_i] == ' ')
        teaSyntaxError(line, filename, "Invalid array definition.");
    for (; sg_i < statementSize; ++sg_i)
    {
        if (statement[sg_i] == ' ' || sg_i == statementSize - 1U)
        {
            if (sg_i == statementSize - 1U)
                sg_element.push_back(statement[sg_i]);
            sg_data.push_back(std::make_any<tea::TeaInt>("", std::stoi(sg_element)));
            sg_element.clear();
            if (sg_i != statementSize - 1U && statement[sg_i + 1U] == ' ')
                teaSyntaxError(line, filename, "Invalid array definition.");
            continue;
        }
        sg_element.push_back(statement[sg_i]);
    }
    return sg_data;
}

// Extracts float data
// std::any is meant to be replaced with TeaFloat
static const std::vector<std::any> getFloatData(const std::string &statement, const std::size_t &statementSize,
    const int &line, const char *&filename)
{
    sg_element.clear();
    sg_data.clear();
    if (statement[++sg_i] == ' ')
        teaSyntaxError(line, filename, "Invalid array definition.");
    for (; sg_i < statementSize; ++sg_i)
    {
        if (statement[sg_i] == ' ' || sg_i == statementSize - 1U)
        {
            if (sg_i == statementSize - 1U)
                sg_element.push_back(statement[sg_i]);
            sg_data.push_back(std::make_any<tea::TeaFloat>("", std::stof(sg_element)));
            sg_element.clear();
            if (sg_i != statementSize - 1U && statement[sg_i + 1U] == ' ')
                teaSyntaxError(line, filename, "Invalid array definition.");
            continue;
        }
        sg_element.push_back(statement[sg_i]);
    }
    return sg_data;
}

// Called when the array keyword is called in tea
void kArray(const std::string &statement, const int &line, const char *&filename, tea::teaArray_t &teaArrays,
    const std::string &currentNamespace)
{
    const std::size_t &&statementSize{statement.size()};
    const std::string &type{getArrayType(statement, statementSize, line, filename)};
    const int &&size{getArraySize(statement, statementSize, line, filename)};
    createvar(getPreArrayName(statement, statementSize, line, filename), line, filename, 0);
    const std::string &&name{currentNamespace + g_varname};
    // Array type is string
    if (type == "string")
    {
        const std::vector<std::any> &&data{getStringData(statement, statementSize, line, filename)};
        assertArraySize(data.size(), size, line, filename);
        teaArrays.push_back(tea::TeaArray{name, type, size, data});
        return;
    }
    // Array type is int
    if (type == "int")
    {
        const std::vector<std::any> &&data{getIntData(statement, statementSize, line, filename)};
        assertArraySize(data.size(), size, line, filename);
        teaArrays.push_back(tea::TeaArray{name, type, size, data});
        return;
    }
    // Array type is float
    if (type == "float")
    {
        const std::vector<std::any> &&data{getFloatData(statement, statementSize, line, filename)};
        assertArraySize(data.size(), size, line, filename);
        teaArrays.push_back(tea::TeaArray{name, type, size, data});
        return;
    }
    teaSyntaxError(line, filename, "Invalid array type.");
}
