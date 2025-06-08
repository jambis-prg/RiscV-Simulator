#include <glfw_custom_allocator.h>
#include <logger.h>

namespace RSCV
{
    void *GLFWCustomAllocator::Allocate(size_t size, void *user)
    {
        void* ptr = malloc(size);

#ifndef NDEBUG
        if (ptr != nullptr)
        {
            std::lock_guard<std::mutex> lock(s_MemMapMutex); // Trava o mutex
            s_MemMap[ptr] = (MemInfo) {.Size = size}; // Adiciona o ponteiro ao map
        } // Libera mutex
#endif

        return ptr;
    }

    void* GLFWCustomAllocator::Reallocate(void *block, size_t size, void *user)
    {
        void *ptr = realloc(block, size);
        
#ifndef NDEBUG
        if (ptr != nullptr)
        {
            std::lock_guard<std::mutex> lock(s_MemMapMutex); // Trava mutex
            s_MemMap.erase(block); // Apaga antigo ponteiro do mapeamento
            s_MemMap[ptr] = (MemInfo) {.Size = size}; // Insere novo ponteiro e novo tamanho no mapeamento
        } // Libera mutex
#endif

        return ptr;
    }

    void GLFWCustomAllocator::Free(void *block, void *user)
    {
#ifndef NDEBUG
        {
            std::lock_guard<std::mutex> lock(s_MemMapMutex); // Trava mutex
            s_MemMap.erase(block); // Apaga ponteiro do mapeamento
        } // Libera mutex
#endif
        free(block);
    }

    void GLFWCustomAllocator::Dump()
    {
#ifndef NDEBUG
        std::lock_guard<std::mutex> lock(s_MemMapMutex); // Trava mutex

        for (auto &[ptr, memInfo] : s_MemMap)
            RSCV_LOG_DEBUG("End.: [%x], Size: [%lu]", ptr, memInfo); // Printa o ponteiro e o tamanho alocado de forma
#endif
    } // Libera mutex
}
