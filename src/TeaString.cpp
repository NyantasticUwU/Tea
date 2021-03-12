#include "TeaString.hpp"

// Contains all necessities for the Tea C++ API
namespace tea
{
    // Default constructor
    TeaString::TeaString(const std::string &name, const std::string &value) : m_name{name}, m_value{value} {}
    // Default deconstructor
    TeaString::~TeaString() {}

    // Gets name
    const std::string &TeaString::getname() const noexcept { return m_name; }
    // Gets value
    const std::string &TeaString::getvalue() const noexcept { return m_value; }
    // Sets type
    const char *TeaString::gettype() const noexcept { return m_type; }

    // Sets name
    TeaString &TeaString::setname(const std::string &newname)
    {
        m_name = newname;
        return *this;
    }
    // Sets value
    TeaString &TeaString::setvalue(const std::string &newvalue)
    {
        m_value = newvalue;
        return *this;
    }
}
