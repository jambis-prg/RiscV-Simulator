#pragma once

#include <stdint.h>
#include <array>

namespace RSCV::CPU
{
    class RegisterContainer
    {
    private:
        inline static uint32_t s_Dummy = 0;
        std::array<uint32_t, 32> m_Regs = { 0 };
    public:
        inline void Reset() { for (auto& reg : m_Regs) reg = 0; }

        inline auto size() const { return m_Regs.size(); }
        
        inline auto begin() { return m_Regs.begin(); }
        inline auto end() { return m_Regs.end(); }

        inline const auto begin() const { return m_Regs.begin(); }
        inline const auto end() const { return m_Regs.end(); }

        inline const auto cbegin() const { return m_Regs.cbegin(); }
        inline const auto cend() const { return m_Regs.cend(); }

        inline uint32_t& operator[](uint8_t index) { return index == 0 ? s_Dummy : m_Regs[index]; }
        inline const uint32_t& operator[](uint8_t index) const { return m_Regs[index]; }
    };
}