#include "createvar.hpp"
#include "error.hpp"
#include "k_if.hpp"
#include "k_snippet.hpp"

// Declaring external globals for defining variables in tea
extern std::string g_varname;

// Called when the snippet keyword is called in tea
void kSnippet(
	const std::vector<std::string> &teafile, const std::string &statement, int &line, const char *&filename,
	teaSnippet_t &teaSnippets)
{
	static std::string s_name;
	static teaSnippetBody_t s_body;
	s_body.clear();
	// Getting return type
	createvar(statement + ' ', line, filename, 7);
	s_name = g_varname;
	// Getting body
	bool &&isEndFound{false};
	int &&blockCount{0};
	int &&teafileSize{static_cast<int>(teafile.size())};
	while (line < teafileSize)
	{
		if (teafile[line] == "end")
		{
			if (!blockCount--)
			{
				isEndFound = true;
				break;
			}
		}
		if (isEnteringBlock(teafile[line]))
			++blockCount;
		s_body.push_back(teafile[line]);
		++line;
	}
	// Checking if end keyword is found
	if (!isEndFound)
		teaSyntaxError(line, filename, "Snippet body never ended (closed).");
	// Time to head out
	++line;
	teaSnippets.push_back(TeaSnippet{s_name, s_body});
}
