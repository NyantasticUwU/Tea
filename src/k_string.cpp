#include "k_string.hpp"
#include "throwerror.hpp"

// This function is called whenever the string keyword is used
void kString(std::string &statement, std::vector<std::pair<std::string, std::string>> &t_strings)
{
    std::string ostatement{statement};
    if (std::count(statement.begin(), statement.end(), ' ') <= 1)
        throwError(statement);
    statement.erase(0, statement.find_first_of(' '));
    statement.erase(0, statement.find_first_not_of(' '));
    const std::string strname{statement.substr(0, statement.find_first_of(' '))};
    if (std::find_if(t_strings.begin(), t_strings.end(), [&](std::pair<std::string, std::string> &p){
        return strname == p.first;
    }) != t_strings.end())
        throwError(ostatement);
    statement.erase(0, statement.find_first_of(' '));
    statement.erase(0, statement.find_first_not_of(' '));
    if (std::count(statement.begin(), statement.end(), '"') <= 1)
        throwError(ostatement);
    const std::string strval{statement.substr(statement.find_first_of('"') + 1, statement.find_last_of('"') - 1)};
    t_strings.push_back(std::pair<std::string, std::string>(strname, strval));
}
