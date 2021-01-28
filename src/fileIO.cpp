#include "error.hpp"
#include "fileIO.hpp"
#include <filesystem>

// Makes sure the .tea file actually exists
void checkFile(const char *&filename)
{
    // Making sure the file is regular (not a directory or anything crazy)
    if (!std::filesystem::is_regular_file(static_cast<const std::filesystem::path>(filename)))
        teaError("ERROR: The given file name doesn't seem to be a regular file.\n"
            "Exiting with code 222 (ERROR_BAD_FILE_TYPE).",
            222); // Windows' ERROR_BAD_FILE_TYPE (222)
}
