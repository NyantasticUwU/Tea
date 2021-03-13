#include "TeaThread.hpp"
#include "terminate.hpp"
#include <vector>

// Declaring globals
extern std::vector<TeaThread> g_threads;

// Contains all necessities for the Tea C++ API
namespace tea
{
    // Cleans up tea
    void terminateTea()
    {
        const std::size_t &&threadsSize{g_threads.size()};
        for (std::size_t &&i{0U}; i < threadsSize; ++i)
        {
            std::thread *&thread{g_threads[i].threadptr};
            if (thread)
            {
                try { thread->detach(); }
                catch (...) {}
                delete thread;
                thread = nullptr;
            }
        }
        g_threads.clear();
    }
}
