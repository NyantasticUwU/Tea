#include "TeaArray.hpp"

// Default constructor
TeaArray::TeaArray(const std::string &name, const std::string &type, const int &size,
    const std::vector<std::any> &data) : m_name{name}, m_type{type}, m_size{size}
{
    m_data = data;
}
// Default deconstructor
TeaArray::~TeaArray() {}

// Operator[]
const std::any &TeaArray::operator[](const int &index) const noexcept { return m_data[index]; }

// Gets name
const std::string &TeaArray::getname() const noexcept { return m_name; }
// Gets type
const std::string &TeaArray::gettype() const noexcept { return m_type; }
// Gets size
const int &TeaArray::getsize() const noexcept { return m_size; }
// Gets data
const std::vector<std::any> &TeaArray::getdata() const noexcept { return m_data; }

// Sets name
TeaArray &TeaArray::setname(const std::string &newname)
{
    m_name = newname;
    return *this;
}
// Sets type
TeaArray &TeaArray::settype(const std::string &newtype)
{
    m_type = newtype;
    return *this;
}
// Sets size
TeaArray &TeaArray::setsize(const int &newsize) noexcept
{
    m_size = newsize;
    return *this;
}
// Sets data
TeaArray &TeaArray::setdata(const std::vector<std::any> &newdata)
{
    m_data = newdata;
    return *this;
}
