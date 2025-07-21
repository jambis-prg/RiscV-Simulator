#pragma once

#include "pipeline.h"
#include <array>

namespace RSCV::CPU
{
    class SingleCycle : public Pipeline
    {
    private:
    public:
        uint32_t Cycle(std::array<uint8_t, 1024> &ram, RegisterContainer &regs, const StoreBranchInstruction &extractedIns) override;
        uint32_t Cycle(std::array<uint8_t, 1024> &ram, RegisterContainer &regs, const RegisterInstruction &extractedIns) override;
        uint32_t Cycle(std::array<uint8_t, 1024> &ram, RegisterContainer &regs, uint32_t PC, const ImmediateInstruction &extractedIns) override;
        uint32_t Cycle(std::array<uint8_t, 1024> &ram, RegisterContainer &regs, uint32_t PC, const UpperImmediateInstruction &extractedIns) override;
        uint32_t Cycle(std::array<uint8_t, 1024> &ram, RegisterContainer &regs, uint32_t PC, uint8_t regDest, uint32_t immediate) override;
        void Reset() override {};
    };
}