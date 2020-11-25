#include "TeaInt.hpp"

// Default constructor
TeaInt::TeaInt(const std::string &name, const int &value) : m_name{name}, m_value{value} {}
// Default deconstructor
TeaInt::~TeaInt() {}

// Gets name
const std::string &TeaInt::getname() const noexcept { return m_name; }
// Gets value
const int &TeaInt::getvalue() const noexcept { return m_value; }
// Gets type
const char *TeaInt::gettype() const noexcept { return m_type; }

// Sets name
TeaInt &TeaInt::setname(const std::string &newname)
{
    m_name = newname;
    return *this;
}
// Sets value
TeaInt &TeaInt::setvalue(const int &newvalue) noexcept
{
    m_value = newvalue;
    return *this;
}
