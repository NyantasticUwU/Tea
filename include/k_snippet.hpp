#ifndef TEA_K_SNIPPET_HPP_INCLUDED
#define TEA_K_SNIPPET_HPP_INCLUDED
#include "TeaSnippet.hpp"

// Called when the snippet keyword is called in tea
void kSnippet(const std::vector<std::string> &teafile, const std::string &statement, int &line,
    const char *&filename, tea::teaSnippet_t &teaSnippets, const std::string &currentNamespace);

#endif
