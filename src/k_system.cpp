#include "k_system.hpp"

// This function is called when the system keyword is called
void kSystem(std::string &statement)
{
    statement.erase(0, statement.find_first_of('"') + 1);
    statement.erase(statement.find_last_of('"'));
    system(statement.c_str());
}
