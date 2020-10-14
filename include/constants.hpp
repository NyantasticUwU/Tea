#ifndef TEA_CONSTANTS_HPP_INCLUDED
#define TEA_CONSTANTS_HPP_INCLUDED
#define MEMORY 0
#define SPEED 1
#define MEMORY_OR_SPEED SPEED
#include <cstdint>
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
#endif
