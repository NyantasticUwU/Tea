#include "k_system.hpp"
#include "runtea.hpp"
#include "throwerror.hpp"

// This function is used to run the main .tea (parsed) file
void runTea(std::vector<std::string> &parsedFile)
{
    std::system("cls");
    for (std::string &statement : parsedFile)
    {
        if (statement.find("system") == 0)
        {
            kSystem(statement);
            continue;
        }
        else
            throwError(statement);
    }
}
