#ifndef TEA_TEA_STRING_HPP_INCLUDED
#define TEA_TEA_STRING_HPP_INCLUDED
#include <string>
#include <vector>

// Represents a tea string (holds string variable name and literal)
class TeaString final
{
    std::string m_name;
    std::string m_value;

public:
    // Default constructor
    TeaString(const std::string &name = "", const std::string &value = "");
    // Default deconstructor
    ~TeaString();

    // Gets name
    const std::string &getname() const noexcept;
    // Gets value
    const std::string &getvalue() const noexcept;

    // Sets name
    TeaString &setname(const std::string &newname);
    // Sets value
    TeaString &setvalue(const std::string &newvalue);
};

// Type def for tea string vector
using teaString_t = std::vector<TeaString>;

#endif