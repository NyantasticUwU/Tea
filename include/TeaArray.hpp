#ifndef TEA_TEA_ARRAY_HPP_INCLUDED
#define TEA_TEA_ARRAY_HPP_INCLUDED
#include <any>
#include <string>
#include <vector>

// Represents a fixed-length array in tea
class TeaArray final
{
    std::string m_name;
    std::string m_type;
    int m_size;
    std::vector<std::any> m_data;

public:
    // Default constructor
    TeaArray(const std::string &name = {}, const std::string &type = {}, const int &size = 0,
        const std::vector<std::any> &data = {});
    // Default deconstructor
    ~TeaArray();

    // Operator[]
    const std::any &operator[](const int &index) const noexcept;

    // Gets name
    const std::string &getname() const noexcept;
    // Gets type
    const std::string &gettype() const noexcept;
    // Gets size
    const int &getsize() const noexcept;
    // Gets data
    const std::vector<std::any> &getdata() const noexcept;

    // Sets name
    TeaArray &setname(const std::string &newname);
    // Sets type
    TeaArray &settype(const std::string &newtype);
    // Sets size
    TeaArray &setsize(const int &newsize) noexcept;
    // Sets data
    TeaArray &setdata(const std::vector<std::any> &newdata);
};

// Type def for tea array vector
using teaArray_t = std::vector<TeaArray>;

#endif
