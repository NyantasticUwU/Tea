#include "k_system.hpp"
#include "runtea.hpp"

// This function is used to run the main .tea (parsed) file
void runTea(std::vector<std::string> &parsedFile)
{
    for (std::string &statement : parsedFile)
    {
        if (statement.find("system") == 0)
            kSystem(statement);
    }
}
