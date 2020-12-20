#include "error.hpp"
#include "k_exit.hpp"

// Called when tea keyword is called in tea
void kExit(const std::string &statement, const int &line, const char *&filename)
{
    std::exit([&]() -> const int {
        const std::size_t statementSize{statement.size()};
        std::string returnString;
        for (std::size_t i{5}; i < statementSize; ++i)
        {
            if (statement[i] == ' ')
                teaSyntaxError(line, filename, "Invalid exit code.");
            returnString.push_back(statement[i]);
        }
        return std::stoi(returnString);
    }());
}
