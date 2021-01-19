#include "error.hpp"
#include "k_exit.hpp"

// Called when tea keyword is called in tea
void kExit(const std::string &statement)
{
    std::exit(std::stoi(statement.substr(5U)));
}
