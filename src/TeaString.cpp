#include "TeaString.hpp"

// Default constructor
TeaString::TeaString(const std::string &_name, const std::string &_value) : name{_name}, value{_value} {}
// Default deconstructor
TeaString::~TeaString() {}

// Gets name
const std::string &TeaString::getname() const { return name; }
// Gets value
const std::string &TeaString::getvalue() const { return value; }

// Sets name
void TeaString::setname(const std::string &newname) { name = newname; }
// Sets value
void TeaString::setvalue(const std::string &newvalue) { value = newvalue; }
