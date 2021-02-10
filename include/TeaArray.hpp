#ifndef TEA_TEA_ARRAY_HPP_INCLUDED
#define TEA_TEA_ARRAY_HPP_INCLUDED
#include <any>
#include <string>
#include <vector>

// Represents a fixed-length array in tea
template <typename T>
class TeaArray final
{
    std::string m_name;
    std::string m_type;
    int m_size;
    std::vector<T> m_data;

public:
    // Default constructor
    TeaArray(const std::string &name = {}, const std::string &type = {}, const int &size = 0,
        const std::vector<T> &data = {}) : m_name{name}, m_type{type}, m_size{size}
    {
        m_data = data;
    }
    // Default deconstructor
    ~TeaArray() {}

    // Operator[]
    const T &operator[](const int &index) const noexcept { return m_data[index]; }

    // Gets name
    const std::string &getname() const noexcept { return m_name; }
    // Gets type
    const std::string &gettype() const noexcept { return m_type; }
    // Gets size
    const int &getsize() const noexcept { return m_size; }
    // Gets data
    const std::vector<T> &getdata() const noexcept { return m_data; }

    // Sets name
    TeaArray &setname(const std::string &newname)
    {
        m_name = newname;
        return *this;
    }
    // Sets type
    TeaArray &settype(const std::string &newtype)
    {
        m_type = newtype;
        return *this;
    }
    // Sets size
    TeaArray &setsize(const int &newsize) noexcept
    {
        m_size = newsize;
        return *this;
    }
    // Sets data
    TeaArray &setdata(const std::vector<T> &newdata)
    {
        m_data = newdata;
        return *this;
    }
};

// Type def for tea array vector
using teaArray_t = std::vector<TeaArray<std::any>>;

#endif
