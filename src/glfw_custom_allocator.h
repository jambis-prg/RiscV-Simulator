#pragma once

#include <stdlib.h>
#include <stdint.h>
#include <unordered_map>
#include <mutex>

namespace RSCV
{

#ifndef NDEBUG
    struct MemInfo
    {
        size_t Size;
    };
#endif
    
    class GLFWCustomAllocator
    {
    private:
#ifndef NDEBUG
        inline static std::unordered_map<void*, MemInfo> s_MemMap;
        inline static std::mutex s_MemMapMutex;
#endif
    public:
        static void* Allocate(size_t size, void* user);
        static void* Reallocate(void* block, size_t size, void* user);
        static void Free(void* block, void* user);

        static void Dump();
    };

#ifdef NDEBUG
    #define RSCV_GLFW_DUMP_MEM()
#else
    #define RSCV_GLFW_DUMP_MEM() GLFWCustomAllocator::Dump()
#endif
}