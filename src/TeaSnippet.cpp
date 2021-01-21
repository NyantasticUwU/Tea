#include "TeaSnippet.hpp"

// Default constructor
TeaSnippet::TeaSnippet(const std::string &name, const teaSnippetBody_t &body) : m_name{name}, m_body{body} {}
// Default deconstructor
TeaSnippet::~TeaSnippet() {}

// Gets name
const std::string &TeaSnippet::getname() const noexcept { return m_name; }
// Gets body
const teaSnippetBody_t &TeaSnippet::getbody() const noexcept { return m_body; }
// Gets type
const char *TeaSnippet::gettype() const noexcept { return m_type; }

// Sets name
TeaSnippet &TeaSnippet::setname(const std::string &newname)
{
    m_name = newname;
    return *this;
}
// Sets body
TeaSnippet &TeaSnippet::setbody(const teaSnippetBody_t &newbody)
{
    m_body = newbody;
    return *this;
}
