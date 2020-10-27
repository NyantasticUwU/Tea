#ifndef TEA_PARSE_FILE_HPP_INCLUDED
#define TEA_PARSE_FILE_HPP_INCLUDED
#include <string>
#include <vector>

// Parses given file (c-str) into a string vector containing each line in the file
void parseFile(const char *&filename, std::vector<std::string> &file);

#endif
