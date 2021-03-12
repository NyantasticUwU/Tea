#ifndef TEA_TEA_FLOAT_HPP_INCLUDED
#define TEA_TEA_FLOAT_HPP_INCLUDED
#include <string>
#include <vector>

// Contains all necessities for the Tea C++ API
namespace tea
{
    // Represents a tea float (holds string name and float literal)
    class TeaFloat final
    {
        std::string m_name;
        float m_value;
        const char *m_type{"float"};

    public:
        // Default constructor
        TeaFloat(const std::string &name = "", const float &value = 0.0f);
        // Default deconstructor
        ~TeaFloat();

        // Gets name
        const std::string &getname() const noexcept;
        // Gets value
        const float &getvalue() const noexcept;
        // Gets type
        const char *gettype() const noexcept;

        // Sets name
        TeaFloat &setname(const std::string &name);
        // Sets value
        TeaFloat &setvalue(const float &newvalue) noexcept;
    };

    // Type def for tea float vector
    using teaFloat_t = std::vector<TeaFloat>;
}

#endif
