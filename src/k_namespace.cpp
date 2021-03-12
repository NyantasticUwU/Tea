#include "createvar.hpp"
#include "k_namespace.hpp"
#include "runtea.hpp"

// Declaring external globals for defining variables in tea
extern thread_local std::string g_varname;

// Called when the namespace keyword is called in tea
void kNamespace(const std::vector<std::string> &teafile, const std::string &statement, int &line,
    const char *&filename, const std::string &currentNamespace, tea::teaString_t &teaStrings,
    tea::teaInt_t &teaInts, tea::teaFloat_t &teaFloats, tea::teaSnippet_t &teaSnippets, tea::teaArray_t &teaArrays)
{
    createvar(statement + ' ', line, filename, 9);
    const std::string &&newNamespace{g_varname + "::"};
    tea::loopTeaStatements(teafile, line, filename, teaStrings, teaInts, teaFloats, teaSnippets, teaArrays,
        currentNamespace + newNamespace);
}
