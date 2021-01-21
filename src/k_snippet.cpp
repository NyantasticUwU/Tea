#include "createvar.hpp"
#include "error.hpp"
#include "k_if.hpp"
#include "k_snippet.hpp"

// Declaring external globals for defining variables in tea
extern std::string g_varname;

// Called when the snippet keyword is called in tea
void kSnippet(const std::vector<std::string> &teafile, const std::string &statement, int &line,
			  const char *&filename, teaSnippet_t &teaSnippets)
{
	static std::string s_name;
	static teaSnippetBody_t s_body;
	static bool s_isEndFound;
	static int s_blockCount;
	static int s_teafileSize;
	// Getting return type
	createvar(statement + ' ', line, filename, 7);
	s_name = g_varname;
	// Getting body
	s_isEndFound = false;
	s_blockCount = 0;
	s_teafileSize = teafile.size();
	while (line < s_teafileSize)
	{
		if (teafile[line] == "end")
		{
			if (!s_blockCount--)
			{
				s_isEndFound = true;
				break;
			}
		}
		if (isEnteringBlock(teafile[line]))
			++s_blockCount;
		s_body.push_back(teafile[line]);
		++line;
	}
	// Checking if end keyword is found
	if (!s_isEndFound)
		teaSyntaxError(line, filename, "Snippet body never ended (closed).");
	// Time to head out
	++line;
	teaSnippets.push_back(TeaSnippet{s_name, s_body});
}
