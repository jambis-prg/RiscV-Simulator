#pragma once

#include <stdint.h>
#include <array>

#include "registers.h"
#include "pipeline.h"
#include "instructions_types.h"

namespace RSCV::CPU
{
    class CPU
    {
    private:
        uint32_t m_PC = 0;
        RegisterContainer m_Regs;
        Pipeline& m_PipelineRef;
    public:
        CPU(Pipeline& pipeline) : m_PipelineRef(pipeline) {}

        void Cycle(std::array<uint8_t, 1024> &ram);

        void Reset();
        inline void SetPipeline(Pipeline& pipeline) { m_PipelineRef = pipeline; m_PipelineRef.Reset(); }

        inline const RegisterContainer& GetRegisters() const { return m_Regs; }
        inline uint32_t GetPC() const { return m_PC; }
    };
}