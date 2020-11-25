#include "TeaFloat.hpp"

// Default constructor
TeaFloat::TeaFloat(const std::string &name, const float &value) : m_name{name}, m_value{value} {}
// Default deconstructor
TeaFloat::~TeaFloat() {}

// Gets name
const std::string &TeaFloat::getname() const noexcept { return m_name; }
// Gets value
const float &TeaFloat::getvalue() const noexcept { return m_value; }
// Gets type
const char *TeaFloat::gettype() const noexcept { return m_type; }

// Sets name
TeaFloat &TeaFloat::setname(const std::string &newname)
{
    m_name = newname;
    return *this;
}
// Sets value
TeaFloat &TeaFloat::setvalue(const float &newvalue) noexcept
{
    m_value = newvalue;
    return *this;
}
