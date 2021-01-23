#include "constants.hpp"
#include "error.hpp"
#include "stdlib.hpp"
#include "TeaStandardSnippet.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>

// Defining static globals
static std::string sg_line;
static std::string sg_read;
static std::ifstream sg_if;
static std::ofstream sg_of;

// Gets variable by name from tea type vector
template <typename T>
static const T &getTeaVariable(const std::vector<T> &typevec, const std::string &varname)
{
    for (const T &t : typevec)
    {
        if (t.getname() == varname)
            return t;
    }
    teaError("Unable to find " + varname + '.', 13);
    return typevec[typevec.size()]; // end
}

// Checks if any vars are equal to tsVariableName
template <typename T>
static const bool isVar(const std::vector<T> &typevec, const std::string &varname)
{
    for (const T &t : typevec)
    {
        if (t.getname() == varname)
            return true;
    }
    return false;
}

// Contains all std snippets
namespace stdSnippet
{
    // Tea standard create directory snippet
    // Takes string tsCreateDirectory
    static void createDirectory(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &)
    {
        std::filesystem::create_directories(getTeaVariable(teaStrings, "tsCreateDirectory").getvalue());
    }

    // Tea standard create file snippet
    // Takes string tsCreateFile
    static void createFile(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &)
    {
        sg_of.open(getTeaVariable(teaStrings, "tsCreateFile").getvalue());
        sg_of.close();
    }

    // Tea standard delete directory snippet
    // Takes string tsDeleteDirectory
    static void deleteDirectory(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &)
    {
        std::filesystem::remove_all(getTeaVariable(teaStrings, "tsDeleteDirectory").getvalue());
    }

    // Tea standard delete file snippet
    // Takes string tsDeleteFile
    static void deleteFile(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &)
    {
        std::filesystem::remove(getTeaVariable(teaStrings, "tsDeleteFile").getvalue());
    }

    // Tea standard file append snippet
    // Takes string tsFileAppendFile, string tsFileAppendOut
    static void fileAppend(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &)
    {
        sg_of.open(getTeaVariable(teaStrings, "tsFileAppendFile").getvalue(), std::ios_base::app);
        sg_of << getTeaVariable(teaStrings, "tsFileAppendOut").getvalue();
        sg_of.close();
    }

    // Tea standard file append line snippet
    // Takes string tsFileAppendLineFile, string tsFileAppendLineOut
    static void fileAppendLine(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &)
    {
        sg_of.open(getTeaVariable(teaStrings, "tsFileAppendLineFile").getvalue(), std::ios_base::app);
        sg_of << getTeaVariable(teaStrings, "tsFileAppendLineOut").getvalue() << '\n';
        sg_of.close();
    }

    // Tea standard file read snippet
    // Takes string tsFileReadFile
    // Outputs string fsFileReadString
    static void fileRead(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &)
    {
        sg_read.clear();
        sg_if.open(getTeaVariable(teaStrings, "tsFileReadFile").getvalue());
        while (std::getline(sg_if, sg_line))
            sg_read.append(sg_line);
        sg_if.close();
        const std::size_t &&readSize{sg_read.size()};
        for (std::size_t &&i{0U}; i < readSize; ++i)
        {
            if (sg_read[i] == '"' || sg_read[i] == '\\')
                sg_read.insert(i++, 1U, '\\');
        }
        teaStrings.push_back({"fsFileReadString", sg_read});
    }

    // Tea standard file read line snippet
    // Takes int tsFileReadLineLine, string tsFileReadLineFile
    // Outputs string fsFileReadLineString
    static void fileReadLine(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &, teaSnippet_t &)
    {
        const std::size_t &&lineToRead{
            static_cast<std::size_t>(getTeaVariable(teaInts, "tsFileReadLineLine").getvalue())};
        std::size_t &&i{0U};
        sg_if.open(getTeaVariable(teaStrings, "tsFileReadLineFile").getvalue());
        while (std::getline(sg_if, sg_line))
        {
            if (++i == lineToRead)
                break;
        }
        sg_if.close();
        const std::size_t &&lineSize{sg_line.size()};
        for (i = 0U; i < lineSize; ++i)
        {
            if (sg_line[i] == '"' || sg_line[i] == '\\')
                sg_line.insert(i++, 1U, '\\');
        }
        teaStrings.push_back({"fsFileReadLineString", sg_line});
    }

    // Tea standard file write snippet
    // Takes string tsFileWriteFile, string tsFileWriteOut
    static void fileWrite(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &)
    {
        sg_of.open(getTeaVariable(teaStrings, "tsFileWriteFile").getvalue());
        sg_of << getTeaVariable(teaStrings, "tsFileWriteOut").getvalue();
        sg_of.close();
    }

    // Tea standard file write line snippet
    // Takes string tsFileWriteLineFile, string tsFileWriteLineOut
    static void fileWriteLine(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &)
    {
        sg_of.open(getTeaVariable(teaStrings, "tsFileWriteLineFile").getvalue());
        sg_of << getTeaVariable(teaStrings, "tsFileWriteLineOut").getvalue() << '\n';
        sg_of.close();
    }

    // Tea standard input snippet
    // Outputs string fsInput
    static void input(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &)
    {
        static std::string s_in;
        std::getline(std::cin, s_in);
        teaStrings.push_back({"fsInput", s_in});
    }

    // Tea standard is variable snippet
    // Takes string tsVariableName
    // Outputs int fsIsVariable
    static void isVariable(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &teaFloats,
                           teaSnippet_t &teaSnippets)
    {
        const std::string &varname{getTeaVariable(teaStrings, "tsVariableName").getvalue()};
        if (isVar(teaStrings, varname) || isVar(teaInts, varname) ||
            isVar(teaFloats, varname) || isVar(teaSnippets, varname))
            teaInts.push_back(TeaInt{"fsIsVariable", 1});
        else
            teaInts.push_back(TeaInt{"fsIsVariable", 0});
    }

    // Tea standard print snippet
    // Takes string tsPrint
    static void print(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &)
    {
        std::cout << getTeaVariable(teaStrings, "tsPrint").getvalue();
    }

    // Tea standard print line snippet
    // Takes string tsPrintLine
    static void printLine(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &)
    {
        std::cout << getTeaVariable(teaStrings, "tsPrintLine").getvalue() << '\n';
    }

    // Tea standard to float snippet
    // Takes int | string tsToFloat
    // Outputs float fsToFloat
    static void toFloat(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &)
    {
        if (isVar(teaInts, "tsToFloat"))
        {
            teaFloats.push_back({"fsToFloat",
                                 static_cast<float>(getTeaVariable(teaInts, "tsToFloat").getvalue())});
            return;
        }
        if (isVar(teaStrings, "tsToFloat"))
        {
            teaFloats.push_back({"fsToFloat", std::stof(getTeaVariable(teaStrings, "tsToFloat").getvalue())});
            return;
        }
        teaError("Unable to convert variable to float.", 13);
    }

    // Tea standard to int snippet
    // Takes float | string tsToInt
    // Outputs fsToInt
    static void toInt(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &)
    {
        if (isVar(teaFloats, "tsToInt"))
        {
            teaInts.push_back({"fsToInt", static_cast<int>(getTeaVariable(teaFloats, "tsToInt").getvalue())});
            return;
        }
        if (isVar(teaStrings, "tsToInt"))
        {
            teaInts.push_back({"fsToInt", std::stoi(getTeaVariable(teaStrings, "tsToInt").getvalue())});
            return;
        }
        teaError("Unable to convert variable to int.", 13);
    }

    // Tea standard to string snippet
    // Takes int | float tsToString
    // Outputs string fsToString
    static void toString(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &)
    {
        if (isVar(teaInts, "tsToString"))
        {
            teaStrings.push_back({"fsToString", std::to_string(getTeaVariable(teaInts, "tsToString").getvalue())});
            return;
        }
        if (isVar(teaFloats, "tsToString"))
        {
            teaStrings.push_back({"fsToString",
                                  std::to_string(getTeaVariable(teaFloats, "tsToString").getvalue())});
            return;
        }
        teaError("Unable to convert variable to string.", 13);
    }
} // namespace stdSnippet

// Defining globals
const TeaStandardSnippet g_teastandardsnippets[TEA_NUMBER_OF_STANDARD_SNIPPETS]{
    {"stdCreateDirectory", stdSnippet::createDirectory},
    {"stdCreateFile", stdSnippet::createFile},
    {"stdDeleteDirectory", stdSnippet::deleteDirectory},
    {"stdDeleteFile", stdSnippet::deleteFile},
    {"stdFileAppend", stdSnippet::fileAppend},
    {"stdFileAppendLine", stdSnippet::fileAppendLine},
    {"stdFileRead", stdSnippet::fileRead},
    {"stdFileReadLine", stdSnippet::fileReadLine},
    {"stdFileWrite", stdSnippet::fileWrite},
    {"stdFileWriteLine", stdSnippet::fileWriteLine},
    {"stdInput", stdSnippet::input},
    {"stdIsVariable", stdSnippet::isVariable},
    {"stdPrint", stdSnippet::print},
    {"stdPrintLine", stdSnippet::printLine},
    {"stdToFloat", stdSnippet::toFloat},
    {"stdToInt", stdSnippet::toInt},
    {"stdToString", stdSnippet::toString}};

// Handles snippet by given name
const bool handleStandardSnippet(const std::string &snippetName, teaString_t &teaStrings, teaInt_t &teaInts,
                                 teaFloat_t &teaFloats, teaSnippet_t &teaSnippets)
{
    for (const TeaStandardSnippet &tss : g_teastandardsnippets)
    {
        if (tss.name == snippetName)
        {
            tss.snippet(teaStrings, teaInts, teaFloats, teaSnippets);
            return true;
        }
    }
    return false;
}
