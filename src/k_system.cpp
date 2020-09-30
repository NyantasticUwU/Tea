#include "k_system.hpp"
#include "throwerror.hpp"
#include <algorithm>

// This function is called when the system keyword is called
void kSystem(std::string &statement)
{
    if (std::count(statement.begin(), statement.end(), '"') <= 1)
        throwError(statement);
    statement.erase(0, statement.find_first_of('"') + 1);
    statement.erase(statement.find_last_of('"'));
    std::system(statement.c_str());
}
