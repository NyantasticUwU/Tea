#ifndef TEA_TEA_STRING_HPP_INCLUDED
#define TEA_TEA_STRING_HPP_INCLUDED
#include <string>
#include <vector>

// Represents a tea string (holds string variable name and literal)
class TeaString final
{
    std::string name;
    std::string value;

public:
    // Default constructor
    TeaString(const std::string &_name = "", const std::string &_value = "");
    // Default deconstructor
    ~TeaString();

    // Gets name
    const std::string &getname() const noexcept;
    // Gets value
    const std::string &getvalue() const noexcept;

    // Sets name
    void setname(const std::string &newname);
    // Sets value
    void setvalue(const std::string &newvalue);
};

// Type def for tea string vector
using teaString_t = std::vector<TeaString>;

#endif
