cmake_minimum_required(VERSION 3.17)
project(Tea)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(include/lib)
link_directories(lib)

add_executable(Tea
        src/cmd.cpp
        src/createvar.cpp
        src/error.cpp
        src/evalops.cpp
        src/fileIO.cpp
        src/init.cpp
        src/k_array.cpp
        src/k_assign.cpp
        src/k_call.cpp
        src/k_declare.cpp
        src/k_delete.cpp
        src/k_elif.cpp
        src/k_else.cpp
        src/k_emplace.cpp
        src/k_exit.cpp
        src/k_float.cpp
        src/k_if.cpp
        src/k_import.cpp
        src/k_include.cpp
        src/k_int.cpp
        src/k_snippet.cpp
        src/k_string.cpp
        src/k_system.cpp
        src/k_while.cpp
        src/kwstr.cpp
        src/parsefile.cpp
        src/runtea.cpp
        src/stdlib.cpp
        src/stringsupport.cpp
        src/TeaFloat.cpp
        src/TeaInt.cpp
        src/TeaSnippet.cpp
        src/TeaString.cpp
        main.cpp)

target_link_libraries(Tea fmt)
