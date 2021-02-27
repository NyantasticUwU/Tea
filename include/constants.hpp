#ifndef TEA_CONSTANTS_HPP_INCLUDED
#define TEA_CONSTANTS_HPP_INCLUDED
#define TEA_NUMBER_OF_OPERATOR_GROUPS 6
#define TEA_VERSION "0.0.0"

// Tea keyword enumeration for indexing g_teakeywords (defined in main.cpp)
enum
{
    TEA_KEYWORD_ARRAY,
    TEA_KEYWORD_ASSIGN,
    TEA_KEYWORD_CALL,
    TEA_KEYWORD_DECLARE,
    TEA_KEYWORD_DELETE,
    TEA_KEYWORD_ELIF,
    TEA_KEYWORD_ELSE,
    TEA_KEYWORD_EMPLACE,
    TEA_KEYWORD_END,
    TEA_KEYWORD_EXIT,
    TEA_KEYWORD_FLOAT,
    TEA_KEYWORD_IF,
    TEA_KEYWORD_IMPORT,
    TEA_KEYWORD_INT,
    TEA_KEYWORD_SNIPPET,
    TEA_KEYWORD_STRING,
    TEA_KEYWORD_SYSTEM,
    TEA_KEYWORD_WHILE,
    TEA_NUMBER_OF_KEYWORDS
};

// Tea standard snippet enumeration for indexing g_teastandardsnippets (defined in stdlib.cpp)
enum
{
    TEA_SNIPPET_STD_ARRAY_APPEND,
    TEA_SNIPPET_STD_ARRAY_CAPACITY,
    TEA_SNIPPET_STD_ARRAY_INSERT,
    TEA_SNIPPET_STD_ARRAY_MAX,
    TEA_SNIPPET_STD_ARRAY_MIN,
    TEA_SNIPPET_STD_ARRAY_REMOVE,
    TEA_SNIPPET_STD_ARRAY_RESERVE,
    TEA_SNIPPET_STD_ARRAY_SHRINK,
    TEA_SNIPPET_STD_ARRAY_SIZE,
    TEA_SNIPPET_STD_ARRAY_TYPE,
    TEA_SNIPPET_STD_COLLECT_GARBAGE,
    TEA_SNIPPET_STD_CREATE_DIRECTORY,
    TEA_SNIPPET_STD_CREATE_FILE,
    TEA_SNIPPET_STD_DELETE_DIRECTORY,
    TEA_SNIPPET_STD_DELETE_FILE,
    TEA_SNIPPET_STD_DIRECTORY_CONTENTS,
    TEA_SNIPPET_STD_EXECUTE,
    TEA_SNIPPET_STD_FILE_APPEND,
    TEA_SNIPPET_STD_FILE_APPEND_LINE,
    TEA_SNIPPET_STD_FILE_GET_LINE_COUNT,
    TEA_SNIPPET_STD_FILE_GET_SIZE,
    TEA_SNIPPET_STD_FILE_READ,
    TEA_SNIPPET_STD_FILE_READ_LINE,
    TEA_SNIPPET_STD_FILE_WRITE,
    TEA_SNIPPET_STD_FILE_WRITE_LINE,
    TEA_SNIPPET_STD_INPUT,
    TEA_SNIPPET_STD_IS_VARIABLE,
    TEA_SNIPPET_STD_PRINT,
    TEA_SNIPPET_STD_PRINT_LINE,
    TEA_SNIPPET_STD_RANDOM_FLOAT,
    TEA_SNIPPET_STD_RANDOM_INT,
    TEA_SNIPPET_STD_REVERSE,
    TEA_SNIPPET_STD_SET_RANDOM_FLOAT_MAX,
    TEA_SNIPPET_STD_SET_RANDOM_FLOAT_MIN,
    TEA_SNIPPET_STD_SET_RANDOM_INT_MAX,
    TEA_SNIPPET_STD_SET_RANDOM_INT_MIN,
    TEA_SNIPPET_STD_SET_RANDOM_SEED,
    TEA_SNIPPET_STD_SHUFFLE,
    TEA_SNIPPET_STD_SLEEP,
    TEA_SNIPPET_STD_SORT,
    TEA_SNIPPET_STD_STRING_LENGTH,
    TEA_SNIPPET_STD_SUB_STRING,
    TEA_SNIPPET_STD_SYSTEM,
    TEA_SNIPPET_STD_TIME,
    TEA_SNIPPET_STD_TIME_LOCAL,
    TEA_SNIPPET_STD_TO_FLOAT,
    TEA_SNIPPET_STD_TO_INT,
    TEA_SNIPPET_STD_TO_STRING,
    TEA_NUMBER_OF_STANDARD_SNIPPETS
};

#endif
