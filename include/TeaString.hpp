#ifndef TEA_TEA_STRING_HPP_INCLUDED
#define TEA_TEA_STRING_HPP_INCLUDED
#include <string>
#include <vector>

// Contains all necessities for the Tea C++ API
namespace tea
{
    // Represents a tea string (holds string variable name and literal)
    class TeaString final
    {
        std::string m_name;
        std::string m_value;
        const char *m_type{"string"};

    public:
        // Default constructor
        TeaString(const std::string &name = "", const std::string &value = "");
        // Default deconstructor
        ~TeaString();

        // Gets name
        const std::string &getname() const noexcept;
        // Gets value
        const std::string &getvalue() const noexcept;
        // Gets type
        const char *gettype() const noexcept;

        // Sets name
        TeaString &setname(const std::string &newname);
        // Sets value
        TeaString &setvalue(const std::string &newvalue);
    };

    // Type def for tea string vector
    using teaString_t = std::vector<TeaString>;
}

#endif
