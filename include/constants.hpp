#ifndef TEA_CONSTANTS_HPP_INCLUDED
#define TEA_CONSTANTS_HPP_INCLUDED
#define MEMORY 0
#define SPEED 1
#define MEMORY_OR_SPEED SPEED
#include "specialfunctions.hpp"
#include <functional>

// Defining efficient int types
#if MEMORY_OR_SPEED == MEMORY
using int8_dynamic_t = std::int_least8_t;
using int16_dynamic_t = std::int_least16_t;
using int32_dynamic_t = std::int_least32_t;
using int64_dynamic_t = std::int_least64_t;
#else
using int8_dynamic_t = std::int_fast8_t;
using int16_dynamic_t = std::int_fast16_t;
using int32_dynamic_t = std::int_fast32_t;
using int64_dynamic_t = std::int_fast64_t;
#endif

// Holds all constants for project
namespace constants
{
    using special_function = std::pair<std::string, std::function<void(std::string &)>>;
    inline const std::vector<std::pair<std::string, std::function<void(std::string &)>>> specialFunctions{
        special_function{"pause", special_functions::sf_pause},
        special_function{"print", special_functions::sf_print},
        special_function{"printline", special_functions::sf_printline}};
} // namespace constants

#endif
