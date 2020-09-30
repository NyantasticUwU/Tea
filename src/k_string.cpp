#include "k_string.hpp"
#include "throwerror.hpp"

// This function is called whenever the string keyword is used
void kString(std::string &statement, std::vector<std::pair<std::string, std::string>> &t_strings)
{
    if (std::count(statement.begin(), statement.end(), ' ') <= 1)
        throwError(statement);
    statement.erase(0, statement.find_first_of(' '));
    statement.erase(0, statement.find_first_not_of(' '));
    const std::string strname{statement.substr(0, statement.find_first_of(' '))};
    statement.erase(0, statement.find_first_of(' '));
    statement.erase(0, statement.find_first_not_of(' '));
    if (std::count(statement.begin(), statement.end(), '"') <= 1)
        throwError(statement);
    const std::string strval{statement.substr(statement.find_first_of('"') + 1, statement.find_last_of('"') - 1)};
    t_strings.push_back(std::pair<std::string, std::string>(strname, strval));
}