#include "k_system.hpp"
#include "kwstr.hpp"

// Called when the system keyword is called in tea
void kSystem(const std::string &statement, const int &line)
{
    std::system(getStringLiteral(statement, line, 6).c_str());
}
