#define _CRT_SECURE_NO_WARNINGS
#include "constants.hpp"
#include "error.hpp"
#include "runtea.hpp"
#include "stdlib.hpp"
#include "TeaStandardSnippet.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <thread>

// Using declarations
using int_dist_t = std::uniform_int_distribution<int>;
using float_dist_t = std::uniform_real_distribution<float>;

// Defining static globals
static const int *sg_pline;
static const char *sg_pfilename;
static const std::string *sg_pcurrentNamespace;
static std::mt19937 sg_randgen{std::random_device{}()};
static int_dist_t sg_intranddist;
static float_dist_t sg_floatranddist;
static std::string sg_line;
static std::ifstream sg_if;
static std::ofstream sg_of;

// Gets variable by name from tea type vector
template <typename T>
static inline const T &getTeaVariable(const std::vector<T> &typevec, const std::string &varname)
{
    for (const T &t : typevec)
    {
        if (t.getname() == varname)
            return t;
    }
    teaSyntaxError(*sg_pline, sg_pfilename, "Unable to find " + varname + '.');
    return *typevec.end();
}

// Checks if any vars are equal to tsVariableName
template <typename T>
static inline const bool isVar(const std::vector<T> &typevec, const std::string &varname)
{
    return std::any_of(
        typevec.begin(),
        typevec.end(),
        [&](const T &t) -> const bool {
            return t.getname() == varname;
        });
}

// Removes duplicate tea types with same names
template <typename T>
static inline void removeDuplicates(std::vector<T> &typevec)
{
    using TeaTypeVectorIterator = typename std::vector<T>::iterator;
    const TeaTypeVectorIterator &&begin{typevec.begin() - 1};
    for (TeaTypeVectorIterator &&i{typevec.end() - 1}; i != begin;)
    {
        const std::string &iteaTypeName{i->getname()};
        for (TeaTypeVectorIterator j{--i}; j != begin; --j)
        {
            if (iteaTypeName == j->getname())
                typevec.erase(j++);
        }
    }
}

// Contains all std snippets
namespace stdSnippet
{
    // Tea standard array append snippet
    // Takes string tsArrayAppend, string|int|float tsArrayAppendValue
    static void arrayAppend(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &,
        teaArray_t &teaArrays)
    {
        const std::string &ts{getTeaVariable(teaStrings, "tsArrayAppend").getvalue()};
        const TeaArray &ta{getTeaVariable(teaArrays, ts)};
        const std::string &arrayType{ta.gettype()};
        std::any newElement;
        if (arrayType == "string")
            newElement = std::make_any<TeaString>(getTeaVariable(teaStrings, "tsArrayAppendValue"));
        else if (arrayType == "int")
            newElement = std::make_any<TeaInt>(getTeaVariable(teaInts, "tsArrayAppendValue"));
        else if (arrayType == "float")
            newElement = std::make_any<TeaFloat>(getTeaVariable(teaFloats, "tsArrayAppendValue"));
        else
            teaSyntaxError(*sg_pline, sg_pfilename, "Invalid array type.");
        const_cast<std::vector<std::any> &>(ta.getdata()).push_back(newElement);
        ++const_cast<int &>(ta.getsize());
    }

    // Tea standard array capacity
    // Takes string tsArrayCapacity
    // Outputs int fsArrayCapacity
    static void arrayCapacity(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &, teaSnippet_t &,
        teaArray_t &teaArrays)
    {
        const std::string &ts{getTeaVariable(teaStrings, "tsArrayCapacity").getvalue()};
        const TeaArray &ta{getTeaVariable(teaArrays, ts)};
        teaInts.push_back({"fsArrayCapacity", static_cast<int>(ta.getdata().capacity())});
    }

    // Tea standard array insert snippet
    // Takes string tsArrayInsert, int tsArrayInsertIndex, string|int|float tsArrayInsertValue
    static void arrayInsert(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &,
        teaArray_t &teaArrays)
    {
        const std::string &ts{getTeaVariable(teaStrings, "tsArrayInsert").getvalue()};
        const int &index{getTeaVariable(teaInts, "tsArrayInsertIndex").getvalue()};
        const TeaArray &ta{getTeaVariable(teaArrays, ts)};
        const std::string &arrayType{ta.gettype()};
        std::any newElement;
        if (arrayType == "string")
            newElement = std::make_any<TeaString>(getTeaVariable(teaStrings, "tsArrayInsertValue"));
        else if (arrayType == "int")
            newElement = std::make_any<TeaInt>(getTeaVariable(teaInts, "tsArrayInsertValue"));
        else if (arrayType == "float")
            newElement = std::make_any<TeaFloat>(getTeaVariable(teaFloats, "tsArrayInsertValue"));
        else
            teaSyntaxError(*sg_pline, sg_pfilename, "Invalid array type.");
        const_cast<std::vector<std::any> &>(ta.getdata()).insert(ta.getdata().begin() + index, newElement);
        ++const_cast<int &>(ta.getsize());
    }

    // Tea standard array max snippet
    // Takes string tsArrayMax
    // Outputs int|float fsArrayMax
    static void arrayMax(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &,
        teaArray_t &teaArrays)
    {
        const std::string &ts{getTeaVariable(teaStrings, "tsArrayMax").getvalue()};
        const TeaArray &ta{getTeaVariable(teaArrays, ts)};
        const std::vector<std::any> &vec{ta.getdata()};
        const std::string &type{ta.gettype()};
        if (type == "int")
        {
            const std::any &a{*std::max_element(vec.begin(), vec.end(),
                [](const std::any &a1, const std::any &a2) -> const bool {
                    return std::any_cast<TeaInt>(a1).getvalue() < std::any_cast<TeaInt>(a2).getvalue();
                })};
            teaInts.push_back({"fsArrayMax", std::any_cast<TeaInt>(a).getvalue()});
            return;
        }
        if (type == "float")
        {
            const std::any &a{*std::max_element(vec.begin(), vec.end(),
                [](const std::any &a1, const std::any &a2) -> const bool {
                    return std::any_cast<TeaFloat>(a1).getvalue() < std::any_cast<TeaFloat>(a2).getvalue();
                })};
            teaFloats.push_back({"fsArrayMax", std::any_cast<TeaFloat>(a).getvalue()});
            return;
        }
        teaSyntaxError(*sg_pline, sg_pfilename, "Could not find valid array for stdArrayMax.");
    }

    // Tea standard array min snippet
    // Takes string tsArrayMin
    // Outputs int|float fsArrayMin
    static void arrayMin(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &,
        teaArray_t &teaArrays)
    {
        const std::string &ts{getTeaVariable(teaStrings, "tsArrayMin").getvalue()};
        const TeaArray &ta{getTeaVariable(teaArrays, ts)};
        const std::vector<std::any> &vec{ta.getdata()};
        const std::string &type{ta.gettype()};
        if (type == "int")
        {
            const std::any &a{*std::max_element(vec.begin(), vec.end(),
                [](const std::any &a1, const std::any &a2) -> const bool {
                    return std::any_cast<TeaInt>(a1).getvalue() > std::any_cast<TeaInt>(a2).getvalue();
                })};
            teaInts.push_back({"fsArrayMin", std::any_cast<TeaInt>(a).getvalue()});
            return;
        }
        if (type == "float")
        {
            const std::any &a{*std::max_element(vec.begin(), vec.end(),
                [](const std::any &a1, const std::any &a2) -> const bool {
                    return std::any_cast<TeaFloat>(a1).getvalue() > std::any_cast<TeaFloat>(a2).getvalue();
                })};
            teaFloats.push_back({"fsArrayMin", std::any_cast<TeaFloat>(a).getvalue()});
            return;
        }
        teaSyntaxError(*sg_pline, sg_pfilename, "Could not find valid array for stdArrayMin.");
    }

    // Tea standard array remove snippet
    // Takes string tsArrayRemove, int tsArrayRemoveIndex
    static void arrayRemove(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &, teaSnippet_t &,
        teaArray_t &teaArrays)
    {
        const std::string &ts{getTeaVariable(teaStrings, "tsArrayRemove").getvalue()};
        const int &index{getTeaVariable(teaInts, "tsArrayRemoveIndex").getvalue()};
        const TeaArray &ta{getTeaVariable(teaArrays, ts)};
        const_cast<std::vector<std::any> &>(ta.getdata()).erase(ta.getdata().begin() + index);
        --const_cast<int &>(ta.getsize());
    }

    // Tea standard array reserve snippet
    // Takes string tsArrayReserve, int tsArrayReserveSize
    static void arrayReserve(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &, teaSnippet_t &,
        teaArray_t &teaArrays)
    {
        const std::string &ts{getTeaVariable(teaStrings, "tsArrayReserve").getvalue()};
        const int &size{getTeaVariable(teaInts, "tsArrayReserveSize").getvalue()};
        const TeaArray &ta{getTeaVariable(teaArrays, ts)};
        const_cast<std::vector<std::any> &>(ta.getdata()).reserve(size);
    }

    // Tea standard array shrink snippet
    // Takes string tsArrayShrink
    static void arrayShrink(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &,
        teaArray_t &teaArrays)
    {
        const std::string &ts{getTeaVariable(teaStrings, "tsArrayShrink").getvalue()};
        const TeaArray &ta{getTeaVariable(teaArrays, ts)};
        const_cast<std::vector<std::any> &>(ta.getdata()).shrink_to_fit();
    }

    // Tea standard array size snippet
    // Takes string tsArraySize
    // Outputs int fsArraySize
    static void arraySize(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &, teaSnippet_t &,
        teaArray_t &teaArrays)
    {
        const std::string &ts{getTeaVariable(teaStrings, "tsArraySize").getvalue()};
        const int &size{getTeaVariable(teaArrays, ts).getsize()};
        teaInts.push_back({"fsArraySize", size});
    }

    // Tea standard array type snippet
    // Takes string tsArrayType
    // Outputs string fsArrayType
    static void arrayType(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &, teaArray_t &teaArrays)
    {
        const std::string &ts{getTeaVariable(teaStrings, "tsArrayType").getvalue()};
        const std::string &fs{getTeaVariable(teaArrays, ts).gettype()};
        teaStrings.push_back({"fsArrayType", fs});
    }

    // Tea standard collect garbage snippet
    static void collectGarbage(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &teaFloats,
        teaSnippet_t &teaSnippets, teaArray_t &teaArrays)
    {
        removeDuplicates(teaStrings);
        removeDuplicates(teaInts);
        removeDuplicates(teaFloats);
        removeDuplicates(teaSnippets);
        removeDuplicates(teaArrays);
    }

    // Tea standard create directory snippet
    // Takes string tsCreateDirectory
    static void createDirectory(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &, teaArray_t &)
    {
        std::filesystem::create_directories(getTeaVariable(teaStrings, "tsCreateDirectory").getvalue());
    }

    // Tea standard create file snippet
    // Takes string tsCreateFile
    static void createFile(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &, teaArray_t &)
    {
        sg_of.open(getTeaVariable(teaStrings, "tsCreateFile").getvalue());
        sg_of.close();
    }

    // Tea standard delete directory snippet
    // Takes string tsDeleteDirectory
    static void deleteDirectory(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &, teaArray_t &)
    {
        std::filesystem::remove_all(getTeaVariable(teaStrings, "tsDeleteDirectory").getvalue());
    }

    // Tea standard delete file snippet
    // Takes string tsDeleteFile
    static void deleteFile(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &, teaArray_t &)
    {
        std::filesystem::remove(getTeaVariable(teaStrings, "tsDeleteFile").getvalue());
    }

    // Tea standard directory contents snippet
    // Takes string tsDirectoryContents
    // Outputs array string[] fsDirectoryContents
    static void directoryContents(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &,
        teaArray_t &teaArrays)
    {
        const std::string &ts{getTeaVariable(teaStrings, "tsDirectoryContents").getvalue()};
        std::filesystem::directory_iterator dir{ts};
        std::vector<std::any> vec;
        for (const std::filesystem::directory_entry &entry : dir)
        {
            const std::string &&p{entry.path().string()};
            vec.push_back(std::make_any<TeaString>("", p));
        }
        const int &&size{static_cast<int>(vec.size())};
        teaArrays.push_back(TeaArray{"fsDirectoryContents", "string", size, vec});
    }

    // Tea standard execute snippet
    // Takes string tsExecute
    static void execute(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &teaFloats,
        teaSnippet_t &teaSnippets, teaArray_t &teaArrays)
    {
        const std::vector<std::string> &&teafile{{getTeaVariable(teaStrings, "tsExecute").getvalue()}};
        int &&line{0};
        const char *&filename{sg_pfilename};
        loopTeaStatements(teafile, line, filename, teaStrings, teaInts, teaFloats, teaSnippets, teaArrays,
            *sg_pcurrentNamespace);
    }

    // Tea standard file append snippet
    // Takes string tsFileAppendFile, string tsFileAppendOut
    static void fileAppend(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &, teaArray_t &)
    {
        sg_of.open(getTeaVariable(teaStrings, "tsFileAppendFile").getvalue(), std::ios_base::app);
        sg_of << getTeaVariable(teaStrings, "tsFileAppendOut").getvalue();
        sg_of.close();
    }

    // Tea standard file append line snippet
    // Takes string tsFileAppendLineFile, string tsFileAppendLineOut
    static void fileAppendLine(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &, teaArray_t &)
    {
        sg_of.open(getTeaVariable(teaStrings, "tsFileAppendLineFile").getvalue(), std::ios_base::app);
        sg_of << getTeaVariable(teaStrings, "tsFileAppendLineOut").getvalue() << '\n';
        sg_of.close();
    }

    // Tea standard file get line count
    // Takes string tsFileGetLineCountFile
    // Outputs int fsFileLineCount
    static void fileGetLineCount(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &, teaSnippet_t &,
        teaArray_t &)
    {
        sg_if.open(getTeaVariable(teaStrings, "tsFileGetLineCountFile").getvalue());
        int &&lineCount{0};
        while (std::getline(sg_if, sg_line))
            ++lineCount;
        sg_if.close();
        if (!sg_line.size())
            ++lineCount;
        teaInts.push_back({"fsFileLineCount", lineCount});
    }

    // Tea standard file get size snippet
    // Takes string tsFileGetSizeFile
    // Outputs int fsFileSize
    static void fileGetSize(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &, teaSnippet_t &, teaArray_t &)
    {
        const std::string &val{getTeaVariable(teaStrings, "tsFileGetSizeFile").getvalue()};
        teaInts.push_back({"fsFileSize", static_cast<int>(std::filesystem::file_size(val))});
    }

    // Tea standard file read snippet
    // Takes string tsFileReadFile
    // Outputs string fsFileReadString
    static void fileRead(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &, teaArray_t &)
    {
        static std::string s_read;
        s_read.clear();
        sg_if.open(getTeaVariable(teaStrings, "tsFileReadFile").getvalue());
        while (std::getline(sg_if, sg_line))
            s_read.append(sg_line + '\n');
        sg_if.close();
        teaStrings.push_back({"fsFileReadString", s_read});
    }

    // Tea standard file read line snippet
    // Takes int tsFileReadLineLine, string tsFileReadLineFile
    // Outputs string fsFileReadLineString
    static void fileReadLine(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &, teaSnippet_t &,
        teaArray_t &)
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
        teaStrings.push_back({"fsFileReadLineString", sg_line});
    }

    // Tea standard file write snippet
    // Takes string tsFileWriteFile, string tsFileWriteOut
    static void fileWrite(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &, teaArray_t &)
    {
        sg_of.open(getTeaVariable(teaStrings, "tsFileWriteFile").getvalue());
        sg_of << getTeaVariable(teaStrings, "tsFileWriteOut").getvalue();
        sg_of.close();
    }

    // Tea standard file write line snippet
    // Takes string tsFileWriteLineFile, string tsFileWriteLineOut
    static void fileWriteLine(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &, teaArray_t &)
    {
        sg_of.open(getTeaVariable(teaStrings, "tsFileWriteLineFile").getvalue());
        sg_of << getTeaVariable(teaStrings, "tsFileWriteLineOut").getvalue() << '\n';
        sg_of.close();
    }

    // Tea standard input snippet
    // Outputs string fsInput
    static void input(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &, teaArray_t &)
    {
        static std::string s_in;
        std::getline(std::cin, s_in);
        teaStrings.push_back({"fsInput", s_in});
    }

    // Tea standard is variable snippet
    // Takes string tsVariableName
    // Outputs int fsIsVariable
    static void isVariable(
        teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &teaSnippets,
        teaArray_t &teaArrays)
    {
        const std::string &varname{getTeaVariable(teaStrings, "tsVariableName").getvalue()};
        if (isVar(teaStrings, varname) || isVar(teaInts, varname) ||
            isVar(teaFloats, varname) || isVar(teaSnippets, varname) ||
            isVar(teaArrays, varname))
            teaInts.push_back(TeaInt{"fsIsVariable", 1});
        else
            teaInts.push_back(TeaInt{"fsIsVariable", 0});
    }

    // Tea standard print snippet
    // Takes string tsPrint
    static void print(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &, teaArray_t &)
    {
        std::cout << getTeaVariable(teaStrings, "tsPrint").getvalue();
    }

    // Tea standard print line snippet
    // Takes string tsPrintLine
    static void printLine(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &, teaArray_t &)
    {
        std::cout << getTeaVariable(teaStrings, "tsPrintLine").getvalue() << '\n';
    }

    // Tea standard random float snippet
    // Outputs fsRandomFloat
    static void randomFloat(teaString_t &, teaInt_t &, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        teaFloats.push_back({"fsRandomFloat", sg_floatranddist(sg_randgen)});
    }

    // Tea standard random int snippet
    // Outputs int fsRandomInt
    static void randomInt(teaString_t &, teaInt_t &teaInts, teaFloat_t &, teaSnippet_t &, teaArray_t &)
    {
        teaInts.push_back({"fsRandomInt", sg_intranddist(sg_randgen)});
    }

    // Tea standard reverse snippet
    // Takes string tsReverse
    static void reverse(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &, teaArray_t &teaArrays)
    {
        const std::string &ts{getTeaVariable(teaStrings, "tsReverse").getvalue()};
        if (isVar(teaStrings, ts))
        {
            std::string &str{const_cast<std::string &>(getTeaVariable(teaStrings, ts).getvalue())};
            std::reverse(str.begin(), str.end());
            return;
        }
        if (isVar(teaArrays, ts))
        {
            const TeaArray &ta{getTeaVariable(teaArrays, ts)};
            std::vector<std::any> &arr{const_cast<std::vector<std::any> &>(ta.getdata())};
            std::reverse(arr.begin(), arr.end());
            return;
        }
        teaSyntaxError(*sg_pline, sg_pfilename, "Array " + ts + " was not found.");
    }

    // Tea stnadard set random float max snippet
    // Takes int tsRandomFloatMax
    static void setRandomFloatMax(teaString_t &, teaInt_t &, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        const float &&a{sg_floatranddist.a()};
        const float &b{getTeaVariable(teaFloats, "tsRandomFloatMax").getvalue()};
        sg_floatranddist.param(float_dist_t::param_type{a, b});
    }

    // Tea standard set random float min snippet
    // Takes int tsRandomFloatMin
    static void setRandomFloatMin(teaString_t &, teaInt_t &, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        const float &a{getTeaVariable(teaFloats, "tsRandomFloatMin").getvalue()};
        const float &&b{sg_floatranddist.b()};
        sg_floatranddist.param(float_dist_t::param_type{a, b});
    }

    // Tea standard set random int max snippet
    // Takes int tsRandomIntMax
    static void setRandomIntMax(teaString_t &, teaInt_t &teaInts, teaFloat_t &, teaSnippet_t &, teaArray_t &)
    {
        const int &&a{sg_intranddist.a()};
        const int &b{getTeaVariable(teaInts, "tsRandomIntMax").getvalue()};
        sg_intranddist.param(int_dist_t::param_type{a, b});
    }

    // Tea standard set random int min snippet
    // Takes int tsRandomIntMin
    static void setRandomIntMin(teaString_t &, teaInt_t &teaInts, teaFloat_t &, teaSnippet_t &, teaArray_t &)
    {
        const int &a{getTeaVariable(teaInts, "tsRandomIntMin").getvalue()};
        const int &&b{sg_intranddist.b()};
        sg_intranddist.param(int_dist_t::param_type{a, b});
    }

    // Tea standard set random seed snippet
    // Takes int tsSeed
    static void setRandomSeed(teaString_t &, teaInt_t &teaInts, teaFloat_t &, teaSnippet_t &, teaArray_t &)
    {
        sg_randgen.seed(getTeaVariable(teaInts, "tsSeed").getvalue());
    }

    // Tea standard shuffle snippet
    // Takes string tsShuffle
    static void shuffle(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &, teaArray_t &teaArrays)
    {
        const std::string &ts{getTeaVariable(teaStrings, "tsShuffle").getvalue()};
        if (isVar(teaStrings, ts))
        {
            std::string &str{const_cast<std::string &>(getTeaVariable(teaStrings, ts).getvalue())};
            std::shuffle(str.begin(), str.end(), sg_randgen);
            return;
        }
        if (isVar(teaArrays, ts))
        {
            const TeaArray &ta{getTeaVariable(teaArrays, ts)};
            std::vector<std::any> &vec{const_cast<std::vector<std::any> &>(ta.getdata())};
            std::shuffle(vec.begin(), vec.end(), sg_randgen);
            return;
        }
        teaSyntaxError(*sg_pline, sg_pfilename, "tsShuffle (string | array) was not found.");
    }

    // Tea standard sleep snippet
    // Takes int tsSleepTime
    static void sleep(teaString_t &, teaInt_t &teaInts, teaFloat_t &, teaSnippet_t &, teaArray_t &)
    {
        const int &t{getTeaVariable(teaInts, "tsSleepTime").getvalue()};
        std::this_thread::sleep_for(static_cast<std::chrono::milliseconds>(t));
    }

    // Tea standard sort snippet
    // Takes string tsSort
    static void sort(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &, teaArray_t &teaArrays)
    {
        const std::string &ts{getTeaVariable(teaStrings, "tsSort").getvalue()};
        if (isVar(teaStrings, ts))
        {
            std::string &str{const_cast<std::string &>(getTeaVariable(teaStrings, ts).getvalue())};
            std::sort(str.begin(), str.end());
            return;
        }
        if (isVar(teaArrays, ts))
        {
            const TeaArray &ta{getTeaVariable(teaArrays, ts)};
            std::vector<std::any> &arr{const_cast<std::vector<std::any> &>(ta.getdata())};
            const std::string &arrt{ta.gettype()};
            if (arrt == "string")
                std::sort(arr.begin(), arr.end(),
                    [&](const std::any &a1, const std::any &a2) -> const bool {
                        return std::any_cast<TeaString>(a1).getvalue() < std::any_cast<TeaString>(a2).getvalue();
                    });
            else if (arrt == "int")
                std::sort(arr.begin(), arr.end(),
                    [&](const std::any &a1, const std::any &a2) -> const bool {
                        return std::any_cast<TeaInt>(a1).getvalue() < std::any_cast<TeaInt>(a2).getvalue();
                    });
            else if (arrt == "float")
                std::sort(arr.begin(), arr.end(),
                    [&](const std::any &a1, const std::any &a2) -> const bool {
                        return std::any_cast<TeaFloat>(a1).getvalue() < std::any_cast<TeaFloat>(a2).getvalue();
                    });
            return;
        }
        teaSyntaxError(*sg_pline, sg_pfilename, "Array " + ts + " was not found.");
    }

    // Tea standard string length snippet
    // Takes string tsStringLength
    // Outputs int fsStringLength
    static void stringLength(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &, teaSnippet_t &,
        teaArray_t &)
    {
        const int &&fs{static_cast<int>(getTeaVariable(teaStrings, "tsStringLength").getvalue().size())};
        teaInts.push_back({"fsStringLength", fs});
    }

    // Tea standard substring snippet
    // Takes string tsSubString, int tsSubStringStart, int tsSubStringCount
    // Outputs string fsSubStrings
    static void subString(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &, teaSnippet_t &, teaArray_t &)
    {
        const int &start{getTeaVariable(teaInts, "tsSubStringStart").getvalue()};
        const int &count{getTeaVariable(teaInts, "tsSubStringCount").getvalue()};
        const std::string &ts{getTeaVariable(teaStrings, "tsSubString").getvalue()};
        teaStrings.push_back({"fsSubString", ts.substr(start, count)});
    }

    // Tea standard system snippet
    // Takes string tsSystem
    // Outputs int fsSystem
    static void system(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &, teaSnippet_t &, teaArray_t &)
    {
        const std::string &ts{getTeaVariable(teaStrings, "tsSystem").getvalue()};
        teaInts.push_back({"fsSystem", std::system(ts.c_str())});
    }

    // Tea standard time snippet
    // Outputs int fsTime
    static void time(teaString_t &, teaInt_t &teaInts, teaFloat_t &, teaSnippet_t &, teaArray_t &)
    {
        teaInts.push_back({"fsTime", static_cast<int>(std::time(nullptr))});
    }

    // Tea standard time local snippet
    // Outputs int fsSecond, int fsMinute, int fsHour, int fsDay, int fsMonth, int fsYear, int fsIsDaylightSaving
    static void timeLocal(teaString_t &, teaInt_t &teaInts, teaFloat_t &, teaSnippet_t &, teaArray_t &)
    {
        std::time_t &&t{std::time(nullptr)};
        std::tm &time{*std::localtime(&t)};
        teaInts.push_back({"fsSecond", time.tm_sec});
        teaInts.push_back({"fsMinute", time.tm_min});
        teaInts.push_back({"fsHour", time.tm_hour});
        teaInts.push_back({"fsDay", time.tm_mday});
        teaInts.push_back({"fsMonth", 1 + time.tm_mon});
        teaInts.push_back({"fsYear", 1900 + time.tm_year});
        teaInts.push_back({"fsIsDaylightSaving", time.tm_isdst});
    }

    // Tea standard to float snippet
    // Takes int | string tsToFloat
    // Outputs float fsToFloat
    static void toFloat(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &,
        teaArray_t &)
    {
        if (isVar(teaInts, "tsToFloat"))
        {
            teaFloats.push_back(
                {"fsToFloat", static_cast<float>(getTeaVariable(teaInts, "tsToFloat").getvalue())});
            return;
        }
        if (isVar(teaStrings, "tsToFloat"))
        {
            teaFloats.push_back({"fsToFloat", std::stof(getTeaVariable(teaStrings, "tsToFloat").getvalue())});
            return;
        }
        teaSyntaxError(*sg_pline, sg_pfilename, "Unable to convert variable to float.");
    }

    // Tea standard to int snippet
    // Takes float | string tsToInt
    // Outputs fsToInt
    static void toInt(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &,
        teaArray_t &)
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
        teaSyntaxError(*sg_pline, sg_pfilename, "Unable to convert variable to int.");
    }

    // Tea standard to string snippet
    // Takes int | float tsToString
    // Outputs string fsToString
    static void toString(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &,
        teaArray_t &)
    {
        if (isVar(teaInts, "tsToString"))
        {
            teaStrings.push_back({"fsToString", std::to_string(getTeaVariable(teaInts, "tsToString").getvalue())});
            return;
        }
        if (isVar(teaFloats, "tsToString"))
        {
            teaStrings.push_back(
                {"fsToString", std::to_string(getTeaVariable(teaFloats, "tsToString").getvalue())});
            return;
        }
        teaSyntaxError(*sg_pline, sg_pfilename, "Unable to convert variable to string.");
    }
} // namespace stdSnippet

// Defining globals
const char *g_teakeywords[TEA_NUMBER_OF_KEYWORDS]
{
    "array",
    "assign",
    "call",
    "declare",
    "delete",
    "elif",
    "else",
    "emplace",
    "end",
    "exit",
    "float",
    "if",
    "import",
    "int",
    "namespace",
    "snippet",
    "string",
    "system",
    "while"
};
const TeaStandardSnippet g_teastandardsnippets[TEA_NUMBER_OF_STANDARD_SNIPPETS]
{
    {"Tea::Array::Append", stdSnippet::arrayAppend},
    {"Tea::Array::Capacity", stdSnippet::arrayCapacity},
    {"Tea::Array::Insert", stdSnippet::arrayInsert},
    {"Tea::Array::Max", stdSnippet::arrayMax},
    {"Tea::Array::Min", stdSnippet::arrayMin},
    {"Tea::Array::Remove", stdSnippet::arrayRemove},
    {"Tea::Array::Reserve", stdSnippet::arrayReserve},
    {"Tea::Array::Shrink", stdSnippet::arrayShrink},
    {"Tea::Array::Size", stdSnippet::arraySize},
    {"Tea::Array::Type", stdSnippet::arrayType},
    {"Tea::Core::CollectGarbage", stdSnippet::collectGarbage},
    {"Tea::Core::Execute", stdSnippet::execute},
    {"Tea::Core::IsVariable", stdSnippet::isVariable},
    {"Tea::Core::System", stdSnippet::system},
    {"Tea::IO::Directory::Contents", stdSnippet::directoryContents},
    {"Tea::IO::Directory::Create", stdSnippet::createDirectory},
    {"Tea::IO::Directory::Delete", stdSnippet::deleteDirectory},
    {"Tea::IO::File::Append", stdSnippet::fileAppend},
    {"Tea::IO::File::AppendLine", stdSnippet::fileAppendLine},
    {"Tea::IO::File::Create", stdSnippet::createFile},
    {"Tea::IO::File::Delete", stdSnippet::deleteFile},
    {"Tea::IO::File::GetLineCount", stdSnippet::fileGetLineCount},
    {"Tea::IO::File::GetSize", stdSnippet::fileGetSize},
    {"Tea::IO::File::Read", stdSnippet::fileRead},
    {"Tea::IO::File::ReadLine", stdSnippet::fileReadLine},
    {"Tea::IO::File::Write", stdSnippet::fileWrite},
    {"Tea::IO::File::WriteLine", stdSnippet::fileWriteLine},
    {"Tea::IO::Input", stdSnippet::input},
    {"Tea::IO::Print", stdSnippet::print},
    {"Tea::IO::PrintLine", stdSnippet::printLine},
    {"Tea::Random::RandomFloat", stdSnippet::randomFloat},
    {"Tea::Random::RandomInt", stdSnippet::randomInt},
    {"Tea::Random::SetRandomFloatMax", stdSnippet::setRandomFloatMax},
    {"Tea::Random::SetRandomFloatMin", stdSnippet::setRandomFloatMin},
    {"Tea::Random::SetRandomIntMax", stdSnippet::setRandomIntMax},
    {"Tea::Random::SetRandomIntMin", stdSnippet::setRandomIntMin},
    {"Tea::Random::SetRandomSeed", stdSnippet::setRandomSeed},
    {"Tea::Reverse", stdSnippet::reverse},
    {"Tea::Shuffle", stdSnippet::shuffle},
    {"Tea::Sleep", stdSnippet::sleep},
    {"Tea::Sort", stdSnippet::sort},
    {"Tea::StringLength", stdSnippet::stringLength},
    {"Tea::SubString", stdSnippet::subString},
    {"Tea::Time", stdSnippet::time},
    {"Tea::TimeLocal", stdSnippet::timeLocal},
    {"Tea::ToFloat", stdSnippet::toFloat},
    {"Tea::ToInt", stdSnippet::toInt},
    {"Tea::ToString", stdSnippet::toString}
};

// Handles snippet by given name
const bool handleStandardSnippet(const std::string &snippetName, const int &line, const char *&filename,
    teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &teaSnippets,
    teaArray_t &teaArrays, const std::string &currentNamespace)
{
    sg_pline = &line;
    sg_pfilename = filename;
    sg_pcurrentNamespace = &currentNamespace;
    for (const TeaStandardSnippet &tss : g_teastandardsnippets)
    {
        if (tss.name == snippetName)
        {
            tss.snippet(teaStrings, teaInts, teaFloats, teaSnippets, teaArrays);
            return true;
        }
    }
    return false;
}
