#ifndef TEA_TEA_THREAD_HPP_INCLUDED
#define TEA_TEA_THREAD_HPP_INCLUDED
#include <thread>

// Represents a tea thread
struct TeaThread final
{
    std::thread *threadptr;
    int id;
};

#endif
