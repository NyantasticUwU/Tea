#ifndef TEA_TEA_SNIPPET_HPP_INCLUDED
#define TEA_TEA_SNIPPET_HPP_INCLUDED
#include <string>
#include <vector>

// Using type defs
using teaSnippetBody_t = std::vector<std::string>;

// Represents a tea snippet
class TeaSnippet
{
    std::string m_name;
    teaSnippetBody_t m_body;
    const char *m_type{"snippet"};

public:
    // Default constructor
    TeaSnippet(const std::string &name = "", const teaSnippetBody_t &body = {});
    // Default deconstructor
    ~TeaSnippet();

    // Gets name
    const std::string &getname() const noexcept;
    // Gets body
    const teaSnippetBody_t &getbody() const noexcept;
    // Gets type
    const char *gettype() const noexcept;

    // Sets name
    TeaSnippet &setname(const std::string &newname);
    // Sets body
    TeaSnippet &setbody(const teaSnippetBody_t &newbody);
};

// Type def for tea snippet vector
using teaSnippet_t = std::vector<TeaSnippet>;

#endif
