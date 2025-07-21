#pragma once

#include <stdint.h>
#include "instructions_types.h"
#include "registers.h"

namespace RSCV::CPU
{
    class Pipeline
    {
    private:
    public:
        
        virtual uint32_t Cycle(std::array<uint8_t, 1024> &ram, RegisterContainer &regs, const StoreBranchInstruction &extractedIns) = 0;
        virtual uint32_t Cycle(std::array<uint8_t, 1024> &ram, RegisterContainer &regs, const RegisterInstruction &extractedIns) = 0;
        virtual uint32_t Cycle(std::array<uint8_t, 1024> &ram, RegisterContainer &regs, uint32_t PC, const ImmediateInstruction &extractedIns) = 0;
        virtual uint32_t Cycle(std::array<uint8_t, 1024> &ram, RegisterContainer &regs, uint32_t PC, const UpperImmediateInstruction &extractedIns) = 0;
        virtual uint32_t Cycle(std::array<uint8_t, 1024> &ram, RegisterContainer &regs, uint32_t PC, uint8_t regDest, uint32_t immediate) = 0;


        virtual void Reset() = 0;
    };
}