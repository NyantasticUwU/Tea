#ifndef Tea_TEA_INT_HPP_INCLUDED
#define Tea_TEA_INT_HPP_INCLUDED
#include <string>
#include <vector>

// Represents a tea int (holds string name and int literal)
class TeaInt final
{
    std::string m_name;
    int m_value;

public:
    // Default constructor
    TeaInt(const std::string &name = "", const int &value = 0);
    // Default deconstructor
    ~TeaInt();

    // Gets name
    const std::string &getname() const noexcept;
    // Gets value
    const int &getvalue() const noexcept;

    // Sets name
    TeaInt &setname(const std::string &newname);
    // Sets value
    TeaInt &setvalue(const int &newvalue);
};

// Type def for tea int vector
using teaInt_t = std::vector<TeaInt>;

#endif
