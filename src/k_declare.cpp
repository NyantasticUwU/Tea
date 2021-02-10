#include "createvar.hpp"
#include "error.hpp"
#include "k_declare.hpp"

// Makes sure declaration type is valid
static void assertType(const std::string &type, const int &line, const char *&filename)
{
    if (type == "string" || type == "int" || type == "float" || type == "snippet" || type == "array")
        return;
    teaSyntaxError(line, filename, "Invalid declaration type.");
}

// Called when the declare keyword is called in tea
void kDeclare(const std::string &statement, const int &line, const char *&filename)
{
    static std::string s_type;
    s_type.clear();
    std::size_t &&i{8U};
    for (const std::size_t &&statementSize{statement.size()}; i < statementSize; ++i)
    {
        if (statement[i] == ' ')
            break;
        s_type.push_back(statement[i]);
    }
    assertType(s_type, line, filename);
    createvar(statement.substr(i) + ' ', line, filename, 0);
}
