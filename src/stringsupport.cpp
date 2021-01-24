#include "error.hpp"
#include "stringsupport.hpp"

// Defining static globals
static std::string sg_newstr;

// Formats tea string
void formatTeaString(std::string &str)
{
    sg_newstr.clear();
    const std::size_t &&strSize{str.size()};
    for (std::size_t &&i{0U}; i < strSize; ++i)
    {
        if (str[i] == '\\' && str[i + 1U] == '\\')
        {
            sg_newstr.push_back('\\');
            ++i;
            continue;
        }
        if (str[i] == '\\' && str[i + 1U] == 'n')
        {
            sg_newstr.push_back('\n');
            ++i;
            continue;
        }
        if (str[i] == '\\' && str[i + 1U] == '"')
        {
            sg_newstr.push_back('"');
            ++i;
            continue;
        }
        if (str[i] == '\\')
            teaError("Invalid backslash in string " + str + " at index " + std::to_string(i) + '.', 13);
        sg_newstr.push_back(str[i]);
    }
    str = sg_newstr;
}

// Unformats tea string
void unformatTeaString(std::string &str)
{
    sg_newstr.clear();
    const std::size_t &&strSize{str.size()};
    for (std::size_t &&i{0U}; i < strSize; ++i)
    {
        if (str[i] == '\\')
        {
            sg_newstr.append("\\\\");
            continue;
        }
        if (str[i] == '\n')
        {
            sg_newstr.append("\\n");
            continue;
        }
        if (str[i] == '\"')
        {
            sg_newstr.append("\\\"");
            continue;
        }
        sg_newstr.push_back(str[i]);
    }
    str = sg_newstr;
}
