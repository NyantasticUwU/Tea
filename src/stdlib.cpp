#define _CRT_SECURE_NO_WARNINGS
#include "constants.hpp"
#include "error.hpp"
#include "runtea.hpp"
#include "stdlib.hpp"
#include "TeaStandardSnippet.hpp"
#include "TeaThread.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <thread>

// Using declarations
using namespace tea;
using int_dist_t = std::uniform_int_distribution<int>;
using float_dist_t = std::uniform_real_distribution<float>;

// Defining static globals
static thread_local const int *sg_pline;
static thread_local const char *sg_pfilename;
static thread_local const std::string *sg_pcurrentNamespace;
static thread_local std::mt19937 sg_randgen{std::random_device{}()};
static thread_local int_dist_t sg_intranddist;
static thread_local float_dist_t sg_floatranddist;
static thread_local std::string sg_line;
static thread_local std::ifstream sg_if;
static thread_local std::ofstream sg_of;

// Defining globals
std::vector<TeaThread> g_threads;

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
    // Tea standard abs snippet
    // Takes string tsAbs
    // Outputs int|float fsAbs
    static void abs(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &,
        teaArray_t &)
    {
        const std::string &ts{getTeaVariable(teaStrings, "tsAbs").getvalue()};
        if (isVar(teaInts, ts))
            teaInts.push_back({"fsAbs", std::abs(getTeaVariable(teaInts, ts).getvalue())});
        else if (isVar(teaFloats, ts))
            teaFloats.push_back({"fsAbs", std::abs(getTeaVariable(teaFloats, ts).getvalue())});
        else
            teaSyntaxError(*sg_pline, sg_pfilename, "Could not find tsAbs.");
    }

    // Tea standard acos snippet
    // Takes int|float tsAcos
    // Outputs float fsAcos
    static void acos(teaString_t &, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        if (isVar(teaInts, "tsAcos"))
            teaFloats.push_back({"fsAcos",
                static_cast<float>(std::acos(getTeaVariable(teaInts, "tsAcos").getvalue()))});
        else
            teaFloats.push_back({"fsAcos", std::acos(getTeaVariable(teaFloats, "tsAcos").getvalue())});
    }

    // Tea standard acosh snippet
    // Takes int|float tsAcosh
    // Outputs float fsAcosh
    static void acosh(teaString_t &, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        if (isVar(teaInts, "tsAcosh"))
            teaFloats.push_back({"fsAcosh",
                static_cast<float>(std::acosh(getTeaVariable(teaInts, "tsAcosh").getvalue()))});
        else
            teaFloats.push_back({"fsAcosh", std::acosh(getTeaVariable(teaFloats, "tsAcosh").getvalue())});
    }

    // Tea standard acsin snippet
    // Takes int|float tsAsin
    // Outputs float fsAsin
    static void asin(teaString_t &, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        if (isVar(teaInts, "tsAsin"))
            teaFloats.push_back({"fsAsin",
                static_cast<float>(std::asin(getTeaVariable(teaInts, "tsAsin").getvalue()))});
        else
            teaFloats.push_back({"fsAsin", std::asin(getTeaVariable(teaFloats, "tsAsin").getvalue())});
    }

    // Tea standard asinh snippet
    // Takes int|float tsAsinh
    // Outputs float fsAsinh
    static void asinh(teaString_t &, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        if (isVar(teaInts, "tsAsinh"))
            teaFloats.push_back({"fsAsinh",
                static_cast<float>(std::asinh(getTeaVariable(teaInts, "tsAsinh").getvalue()))});
        else
            teaFloats.push_back({"fsAsinh", std::asinh(getTeaVariable(teaFloats, "tsAsinh").getvalue())});
    }

    // Tea standard atan snippet
    // Takes int|float tsAtan
    // Outputs float fsAtan
    static void atan(teaString_t &, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        if (isVar(teaInts, "tsAtan"))
            teaFloats.push_back({"fsAtan",
                static_cast<float>(std::atan(getTeaVariable(teaInts, "tsAtan").getvalue()))});
        else
            teaFloats.push_back({"fsAtan", std::atan(getTeaVariable(teaFloats, "tsAtan").getvalue())});
    }

    // Tea standard atan2 snippet
    // Takes int|float tsAtan2X, int|float tsAtan2Y
    // Outputs float fsAtan2
    static void atan2(teaString_t &, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        if (isVar(teaInts, "tsAtan2X") && isVar(teaInts, "tsAtan2Y"))
        {
            const int &x{getTeaVariable(teaInts, "tsAtan2X").getvalue()};
            const int &y{getTeaVariable(teaInts, "tsAtan2Y").getvalue()};
            teaFloats.push_back({"fsAtan2", static_cast<float>(std::atan2(y, x))});
        }
        else
        {
            const float &x{getTeaVariable(teaFloats, "tsAtan2X").getvalue()};
            const float &y{getTeaVariable(teaFloats, "tsAtan2Y").getvalue()};
            teaFloats.push_back({"fsAtan2", std::atan2(y, x)});
        }
    }

    // Tea standard atanh snippet
    // Takes int|float tsAtanh
    // Outputs float fsAtanh
    static void atanh(teaString_t &, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        if (isVar(teaInts, "tsAtanh"))
            teaFloats.push_back({"fsAtanh",
                static_cast<float>(std::atanh(getTeaVariable(teaInts, "tsAtanh").getvalue()))});
        else
            teaFloats.push_back({"fsAtanh", std::atanh(getTeaVariable(teaFloats, "tsAtanh").getvalue())});
    }

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

    // Tea standard cbrt snippet
    // Takes int|float tsCbrt
    // Outputs float fsCbrt
    static void cbrt(teaString_t &, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        if (isVar(teaInts, "tsCbrt"))
            teaFloats.push_back({"fsCbrt",
                static_cast<float>(std::cbrt(getTeaVariable(teaInts, "tsCbrt").getvalue()))});
        else
            teaFloats.push_back({"fsCbrt", std::cbrt(getTeaVariable(teaFloats, "tsCbrt").getvalue())});
    }

    // Tea standard ceil snippet
    // Takes float tsCeil
    // Outputs int fsCeil
    static void ceil(teaString_t &, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        teaInts.push_back({"fsCeil", static_cast<int>(std::ceil(getTeaVariable(teaFloats, "tsCeil").getvalue()))});
    }

    // Tea standard copy sign snippet
    // Takes int|float tsCopySignX, int|float tsCopySignY
    // Outputs int|float fsCopySign
    static void copySign(teaString_t &, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        if (isVar(teaInts, "tsCopySignX") && isVar(teaInts, "tsCopySignY"))
        {
            const int &x{getTeaVariable(teaInts, "tsCopySignX").getvalue()};
            const int &y{getTeaVariable(teaInts, "tsCopySignY").getvalue()};
            teaInts.push_back({"fsCopySign", static_cast<int>(std::copysign(x, y))});
        }
        else
        {
            const float &x{getTeaVariable(teaFloats, "tsCopySignX").getvalue()};
            const float &y{getTeaVariable(teaFloats, "tsCopySignY").getvalue()};
            teaFloats.push_back({"fsCopySign", std::copysign(x, y)});
        }
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

    // Tea standard cos snippet
    // Takes int|float tsCos
    // Outputs float fsCos
    static void cos(teaString_t &, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        if (isVar(teaInts, "tsCos"))
            teaFloats.push_back({"fsCos",
                static_cast<float>(std::cos(getTeaVariable(teaInts, "tsCos").getvalue()))});
        else
            teaFloats.push_back({"fsCos", std::cos(getTeaVariable(teaFloats, "tsCos").getvalue())});
    }

    // Tea standard cosh snippet
    // Takes int|float tsCosh
    // Outputs float fsCosh
    static void cosh(teaString_t &, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        if (isVar(teaInts, "tsCosh"))
            teaFloats.push_back({"fsCosh",
                static_cast<float>(std::cosh(getTeaVariable(teaInts, "tsCosh").getvalue()))});
        else
            teaFloats.push_back({"fsCosh", std::cosh(getTeaVariable(teaFloats, "tsCosh").getvalue())});
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

    // Tea standard directory exists snippet
    // Takes string tsDirectoryExists
    // Outputs int fsDirectoryExists
    static void directoryExists(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &, teaSnippet_t &,
        teaArray_t &)
    {
        const std::string &dir{getTeaVariable(teaStrings, "tsDirectoryExists").getvalue()};
        teaInts.push_back({"fsDirectoryExists", std::filesystem::is_directory(dir)});
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

    // Tea standard exp snippet
    // Takes int|float tsExp
    // Outputs float fsExp
    static void exp(teaString_t &, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        if (isVar(teaInts, "tsExp"))
            teaFloats.push_back({"fsExp",
                static_cast<float>(std::exp(getTeaVariable(teaInts, "tsExp").getvalue()))});
        else
            teaFloats.push_back({"fsExp", std::exp(getTeaVariable(teaFloats, "tsExp").getvalue())});
    }

    // Tea standard exp2 snippet
    // Takes int|float tsExp2
    // Outputs float fsExp2
    static void exp2(teaString_t &, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        if (isVar(teaInts, "tsExp2"))
            teaFloats.push_back({"fsExp2",
                static_cast<float>(std::exp2(getTeaVariable(teaInts, "tsExp2").getvalue()))});
        else
            teaFloats.push_back({"fsExp2", std::exp2(getTeaVariable(teaFloats, "tsExp2").getvalue())});
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

    // Tea standard file exists snippet
    // Takes string tsFileExists
    // Outputs int fsFileExists
    static void fileExists(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &, teaSnippet_t &, teaArray_t &)
    {
        const std::string &file{getTeaVariable(teaStrings, "tsFileExists").getvalue()};
        teaInts.push_back({"fsFileExists", std::filesystem::is_regular_file(file)});
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
        static thread_local std::string s_read;
        s_read.clear();
        sg_if.open(getTeaVariable(teaStrings, "tsFileReadFile").getvalue());
        while (std::getline(sg_if, sg_line))
            s_read.append(sg_line).push_back('\n');
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

    // Tea standard floor snippet
    // Takes float tsFloor
    // Outputs int fsFloor
    static void floor(teaString_t &, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        teaInts.push_back({"fsFloor",
            static_cast<int>(std::floor(getTeaVariable(teaFloats, "tsFloor").getvalue()))});
    }

    // Tea standard gamma snippet
    // Takes int|float tsGamma
    // Outputs float fsGamma
    static void gamma(teaString_t &, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        if (isVar(teaInts, "tsGamma"))
            teaFloats.push_back({"fsGamma",
                static_cast<float>(std::tgamma(getTeaVariable(teaInts, "tsGamma").getvalue()))});
        else
            teaFloats.push_back({"fsGamma", std::tgamma(getTeaVariable(teaFloats, "tsGamma").getvalue())});
    }

    // Tea standard ilogb snippet
    // Takes int|float tsILogB
    // Outputs int fsILogB
    static void ilogb(teaString_t &, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        if (isVar(teaInts, "tsILogB"))
            teaInts.push_back({"fsILogB", std::ilogb(getTeaVariable(teaInts, "tsILogB").getvalue())});
        else
            teaInts.push_back({"fsILogB", std::ilogb(getTeaVariable(teaFloats, "tsILogB").getvalue())});
    }

    // Tea standard input snippet
    // Outputs string fsInput
    static void input(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &, teaArray_t &)
    {
        static thread_local std::string s_in;
        std::getline(std::cin, s_in);
        teaStrings.push_back({"fsInput", s_in});
    }

    // Tea standard is variable snippet
    // Takes string tsVariableName
    // Outputs int fsIsVariable
    static void isVariable(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &teaFloats,
        teaSnippet_t &teaSnippets, teaArray_t &teaArrays)
    {
        const std::string &varname{getTeaVariable(teaStrings, "tsVariableName").getvalue()};
        if (isVar(teaStrings, varname) || isVar(teaInts, varname) ||
            isVar(teaFloats, varname) || isVar(teaSnippets, varname) ||
            isVar(teaArrays, varname))
            teaInts.push_back(TeaInt{"fsIsVariable", 1});
        else
            teaInts.push_back(TeaInt{"fsIsVariable", 0});
    }

    // Tea standard join snippet
    // Takes int tsThreadID
    static void joinThread(teaString_t &, teaInt_t &teaInts, teaFloat_t &, teaSnippet_t &, teaArray_t &)
    {
        const int &id{getTeaVariable(teaInts, "tsThreadID").getvalue()};
        const std::size_t &&threadsSize{g_threads.size()};
        for (std::size_t &&i{0U}; i < threadsSize; ++i)
        {
            TeaThread &teaThread{g_threads[i]};
            if (teaThread.id == id)
            {
                std::thread *&thread{teaThread.threadptr};
                if (thread)
                {
                    thread->join();
                    delete thread;
                    thread = nullptr;
                    return;
                }
            }
        }
    }

    // Tea standard log snippet
    // Takes int|float tsLog
    // Outputs float fsLog
    static void log(teaString_t &, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        if (isVar(teaInts, "tsLog"))
            teaFloats.push_back({"fsLog",
                static_cast<float>(std::log(getTeaVariable(teaInts, "tsLog").getvalue()))});
        else
            teaFloats.push_back({"fsLog", std::log(getTeaVariable(teaFloats, "tsLog").getvalue())});
    }

    // Tea standard log10 snippet
    // Takes int|float tsLog10
    // Outputs float fsLog10
    static void log10(teaString_t &, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        if (isVar(teaInts, "tsLog10"))
            teaFloats.push_back({"fsLog10",
                static_cast<float>(std::log10(getTeaVariable(teaInts, "tsLog10").getvalue()))});
        else
            teaFloats.push_back({"fsLog10", std::log10(getTeaVariable(teaFloats, "tsLog10").getvalue())});
    }

    // Tea standard log2 snippet
    // Takes int|float tsLog2
    // Outputs float fsLog2
    static void log2(teaString_t &, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        if (isVar(teaInts, "tsLog2"))
            teaFloats.push_back({"fsLog2",
                static_cast<float>(std::log2(getTeaVariable(teaInts, "tsLog2").getvalue()))});
        else
            teaFloats.push_back({"fsLog2", std::log2(getTeaVariable(teaFloats, "tsLog2").getvalue())});
    }

    // Tea standard make inlined snippet
    // Takes string tsMakeInlined
    static void makeInlined(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &teaFloats,
        teaSnippet_t &teaSnippets, teaArray_t &teaArrays)
    {
        static const std::string *s_name;
        const std::string &ts{getTeaVariable(teaStrings, "tsMakeInlined").getvalue()};
        if (isVar(teaStrings, ts))
            s_name = &getTeaVariable(teaStrings, ts).getname();
        else if (isVar(teaInts, ts))
            s_name = &getTeaVariable(teaInts, ts).getname();
        else if (isVar(teaFloats, ts))
            s_name = &getTeaVariable(teaFloats, ts).getname();
        else if (isVar(teaSnippets, ts))
            s_name = &getTeaVariable(teaSnippets, ts).getname();
        else if (isVar(teaArrays, ts))
            s_name = &getTeaVariable(teaArrays, ts).getname();
        else
            teaSyntaxError(*sg_pline, sg_pfilename, "Unable to find " + ts + '.');
        std::string &name{const_cast<std::string &>(*s_name)};
        const std::size_t &&dcpos{name.find_last_of("::") + 1U};
        if (dcpos != name.npos)
            name.erase(name.begin(), name.begin() + dcpos);
    }

    // Tea standard mod snippet
    // Takes int|float tsModX, int|float tsModY
    // Outputs int|float fsMod
    static void mmod(teaString_t &, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        if (isVar(teaInts, "tsModX") && isVar(teaInts, "tsModY"))
        {
            const int &x{getTeaVariable(teaInts, "tsModX").getvalue()};
            const int &y{getTeaVariable(teaInts, "tsModY").getvalue()};
            teaInts.push_back({"fsMod", x % y});
        }
        else
        {
            const float &x{getTeaVariable(teaFloats, "tsModX").getvalue()};
            const float &y{getTeaVariable(teaFloats, "tsModY").getvalue()};
            teaFloats.push_back({"fsMod", std::fmod(x, y)});
        }
    }

    // Tea standard pow snippet
    // Takes int|float tsPowX, int|float tsPowY
    // Outputs float fsPow
    static void pow(teaString_t &, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        if (isVar(teaInts, "tsPowX") && isVar(teaInts, "tsPowY"))
        {
            const int &x{getTeaVariable(teaInts, "tsPowX").getvalue()};
            const int &y{getTeaVariable(teaInts, "tsPowY").getvalue()};
            teaFloats.push_back({"fsPow", static_cast<float>(std::pow(x, y))});
        }
        else
        {
            const float &x{getTeaVariable(teaFloats, "tsPowX").getvalue()};
            const float &y{getTeaVariable(teaFloats, "tsPowY").getvalue()};
            teaFloats.push_back({"fsPow", std::pow(x, y)});
        }
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

    // Tea standard round snippet
    // Takes float tsRound
    // Outputs int fsRound
    static void round(teaString_t &, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        teaInts.push_back({"fsRound",
            static_cast<int>(std::round(getTeaVariable(teaFloats, "tsRound").getvalue()))});
    }

    // Tea stnadard set random float max snippet
    // Takes float tsRandomFloatMax
    static void setRandomFloatMax(teaString_t &, teaInt_t &, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        const float &&a{sg_floatranddist.a()};
        const float &b{getTeaVariable(teaFloats, "tsRandomFloatMax").getvalue()};
        sg_floatranddist.param(float_dist_t::param_type{a, b});
    }

    // Tea standard set random float min snippet
    // Takes float tsRandomFloatMin
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

    // Tea standard sin snippet
    // Takes int|float tsSin
    // Outputs float fsSin
    static void sin(teaString_t &, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        if (isVar(teaInts, "tsSin"))
            teaFloats.push_back({"fsSin",
                static_cast<float>(std::sin(getTeaVariable(teaInts, "tsSin").getvalue()))});
        else
            teaFloats.push_back({"fsSin", std::sin(getTeaVariable(teaFloats, "tsSin").getvalue())});
    }

    // Tea standard sinh snippet
    // Takes int|float tsSinh
    // Outputs float fsSinh
    static void sinh(teaString_t &, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        if (isVar(teaInts, "tsSinh"))
            teaFloats.push_back({"fsSinh",
                static_cast<float>(std::sinh(getTeaVariable(teaInts, "tsSinh").getvalue()))});
        else
            teaFloats.push_back({"fsSinh", std::sinh(getTeaVariable(teaFloats, "tsSinh").getvalue())});
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

    // Tea standard sqrt snippet
    // Takes int|float tsSqrt
    // Outputs float fsSqrt
    static void sqrt(teaString_t &, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        if (isVar(teaInts, "tsSqrt"))
            teaFloats.push_back({"fsSqrt",
                static_cast<float>(std::sqrt(getTeaVariable(teaInts, "tsSqrt").getvalue()))});
        else
            teaFloats.push_back({"fsSqrt", std::sqrt(getTeaVariable(teaFloats, "tsSqrt").getvalue())});
    }

    // Tea standard start thread snippet
    // Takes string tsStartThread
    // Outputs int fsThreadID
    static void startThread(teaString_t &teaStrings, teaInt_t &teaInts, teaFloat_t &, teaSnippet_t &teaSnippets,
        teaArray_t &)
    {
        static int s_threadID{0};
        const std::string &ts{getTeaVariable(teaStrings, "tsStartThread").getvalue()};
        const teaSnippetBody_t &body{getTeaVariable(teaSnippets, ts).getbody()};
        const std::size_t &&napos{ts.find_last_of("::")};
        const std::string &&ns{(napos != ts.npos) ? ts.substr(0U, napos + 1U) : ""};
        std::thread *&&newThread{new std::thread{loopTeaStatementsOnThread, body, ts, ns}};
        g_threads.push_back({newThread, ++s_threadID});
        teaInts.push_back({"fsThreadID", s_threadID});
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

    // Tea standard tan snippet
    // Takes int|float tsTan
    // Outputs float fsTan
    static void tan(teaString_t &, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        if (isVar(teaInts, "tsTan"))
            teaFloats.push_back({"fsTan",
                static_cast<float>(std::tan(getTeaVariable(teaInts, "tsTan").getvalue()))});
        else
            teaFloats.push_back({"fsTan", std::tan(getTeaVariable(teaFloats, "tsTan").getvalue())});
    }

    // Tea standard tanh snippet
    // Takes int|float tsTanh
    // Outputs float fsTanh
    static void tanh(teaString_t &, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        if (isVar(teaInts, "tsTanh"))
            teaFloats.push_back({"fsTanh",
                static_cast<float>(std::tanh(getTeaVariable(teaInts, "tsTanh").getvalue()))});
        else
            teaFloats.push_back({"fsTanh", std::tanh(getTeaVariable(teaFloats, "tsTanh").getvalue())});
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

    // Tea standard truncate snippet
    // Takes int|float tsTruncate
    // Outputs float fsTruncate
    static void truncate(teaString_t &, teaInt_t &teaInts, teaFloat_t &teaFloats, teaSnippet_t &, teaArray_t &)
    {
        if (isVar(teaInts, "tsTruncate"))
            teaFloats.push_back({"fsTruncate",
                static_cast<float>(std::trunc(getTeaVariable(teaInts, "tsTruncate").getvalue()))});
        else
            teaFloats.push_back({"fsTruncate", std::trunc(getTeaVariable(teaFloats, "tsTruncate").getvalue())});
    }

    // Tea standard version snippet
    // Outputs string fsVersion
    static void version(teaString_t &teaStrings, teaInt_t &, teaFloat_t &, teaSnippet_t &, teaArray_t &)
    {
        teaStrings.push_back({"fsVersion", TEA_VERSION});
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
    "inline",
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
    {"Tea::Core::MakeInlined", stdSnippet::makeInlined},
    {"Tea::Core::System", stdSnippet::system},
    {"Tea::Core::Version", stdSnippet::version},
    {"Tea::IO::Directory::Contents", stdSnippet::directoryContents},
    {"Tea::IO::Directory::Create", stdSnippet::createDirectory},
    {"Tea::IO::Directory::Delete", stdSnippet::deleteDirectory},
    {"Tea::IO::Directory::Exists", stdSnippet::directoryExists},
    {"Tea::IO::File::Append", stdSnippet::fileAppend},
    {"Tea::IO::File::AppendLine", stdSnippet::fileAppendLine},
    {"Tea::IO::File::Create", stdSnippet::createFile},
    {"Tea::IO::File::Delete", stdSnippet::deleteFile},
    {"Tea::IO::File::Exists", stdSnippet::fileExists},
    {"Tea::IO::File::GetLineCount", stdSnippet::fileGetLineCount},
    {"Tea::IO::File::GetSize", stdSnippet::fileGetSize},
    {"Tea::IO::File::Read", stdSnippet::fileRead},
    {"Tea::IO::File::ReadLine", stdSnippet::fileReadLine},
    {"Tea::IO::File::Write", stdSnippet::fileWrite},
    {"Tea::IO::File::WriteLine", stdSnippet::fileWriteLine},
    {"Tea::IO::Input", stdSnippet::input},
    {"Tea::IO::Print", stdSnippet::print},
    {"Tea::IO::PrintLine", stdSnippet::printLine},
    {"Tea::Math::Abs", stdSnippet::abs},
    {"Tea::Math::Acos", stdSnippet::acos},
    {"Tea::Math::Acosh", stdSnippet::acosh},
    {"Tea::Math::Asin", stdSnippet::asin},
    {"Tea::Math::Asinh", stdSnippet::asinh},
    {"Tea::Math::Atan", stdSnippet::atan},
    {"Tea::Math::Atan2", stdSnippet::atan2},
    {"Tea::Math::Atanh", stdSnippet::atanh},
    {"Tea::Math::Cbrt", stdSnippet::cbrt},
    {"Tea::Math::Ceil", stdSnippet::ceil},
    {"Tea::Math::CopySign", stdSnippet::copySign},
    {"Tea::Math::Cos", stdSnippet::cos},
    {"Tea::Math::Cosh", stdSnippet::cosh},
    {"Tea::Math::Exp", stdSnippet::exp},
    {"Tea::Math::Exp2", stdSnippet::exp2},
    {"Tea::Math::Floor", stdSnippet::floor},
    {"Tea::Math::Gamma", stdSnippet::gamma},
    {"Tea::Math::ILogB", stdSnippet::ilogb},
    {"Tea::Math::Log", stdSnippet::log},
    {"Tea::Math::Log10", stdSnippet::log10},
    {"Tea::Math::Log2", stdSnippet::log2},
    {"Tea::Math::Mod", stdSnippet::mmod},
    {"Tea::Math::Pow", stdSnippet::pow},
    {"Tea::Math::Round", stdSnippet::round},
    {"Tea::Math::Sin", stdSnippet::sin},
    {"Tea::Math::Sinh", stdSnippet::sinh},
    {"Tea::Math::Sqrt", stdSnippet::sqrt},
    {"Tea::Math::Tan", stdSnippet::tan},
    {"Tea::Math::Tanh", stdSnippet::tanh},
    {"Tea::Math::Truncate", stdSnippet::truncate},
    {"Tea::Random::RandomFloat", stdSnippet::randomFloat},
    {"Tea::Random::RandomInt", stdSnippet::randomInt},
    {"Tea::Random::SetRandomFloatMax", stdSnippet::setRandomFloatMax},
    {"Tea::Random::SetRandomFloatMin", stdSnippet::setRandomFloatMin},
    {"Tea::Random::SetRandomIntMax", stdSnippet::setRandomIntMax},
    {"Tea::Random::SetRandomIntMin", stdSnippet::setRandomIntMin},
    {"Tea::Random::SetRandomSeed", stdSnippet::setRandomSeed},
    {"Tea::Reverse", stdSnippet::reverse},
    {"Tea::Shuffle", stdSnippet::shuffle},
    {"Tea::Sort", stdSnippet::sort},
    {"Tea::StringLength", stdSnippet::stringLength},
    {"Tea::SubString", stdSnippet::subString},
    {"Tea::Thread::Join", stdSnippet::joinThread},
    {"Tea::Thread::Sleep", stdSnippet::sleep},
    {"Tea::Thread::StartThread", stdSnippet::startThread},
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
