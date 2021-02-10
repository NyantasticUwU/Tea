#ifndef TEA_CMD_HPP_INCLUDED
#define TEA_CMD_HPP_INCLUDED
#include "TeaArray.hpp"

// Extracts the file name of the .tea file from the argument vector
const char *&getMainFileName(const int &argc, const char **&argv);

// Fills tea string vector with cmd args
void fillTeaArgs(const int &argc, const char **&argv, teaArray_t &teaArrays);

#endif
