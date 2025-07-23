#include "single_cycle.h"

namespace RSCV::CPU
{
    uint32_t SingleCycle::Cycle(std::array<uint8_t, 1024> &ram, RegisterContainer &regs, const StoreBranchInstruction &extractedIns)
    {
        switch (static_cast<InstructionType>(extractedIns.Opcode))
        {
        case InstructionType::BRANCH:
            switch(extractedIns.Func3)
            {
            case Func3Type::BEQ:
                return regs[extractedIns.Register1] == regs[extractedIns.Register2] ? extractedIns.Immediate : 0;
            case Func3Type::BNE:
                return regs[extractedIns.Register1] != regs[extractedIns.Register2] ? extractedIns.Immediate : 0;
            case Func3Type::BLT:
                return static_cast<int32_t>(regs[extractedIns.Register1]) < static_cast<int32_t>(regs[extractedIns.Register2]) ? extractedIns.Immediate : 0;
            case Func3Type::BGE:
                return static_cast<int32_t>(regs[extractedIns.Register1]) >= static_cast<int32_t>(regs[extractedIns.Register2]) ? extractedIns.Immediate : 0;
            case Func3Type::BLTU:
                return regs[extractedIns.Register1] < regs[extractedIns.Register2] ? extractedIns.Immediate : 0;
            case Func3Type::BGEU:
                return regs[extractedIns.Register1] >= regs[extractedIns.Register2] ? extractedIns.Immediate : 0;
            }
            
            return 0;

        case InstructionType::STORE:
            switch (extractedIns.Func3)
            {
            case Func3Type::SB:
                ram[regs[extractedIns.Register1] + extractedIns.Immediate] = static_cast<uint8_t>(regs[extractedIns.Register2]);
                break;
            case Func3Type::SH:
                ram[regs[extractedIns.Register1] + extractedIns.Immediate] = static_cast<uint8_t>(regs[extractedIns.Register2]);
                ram[regs[extractedIns.Register1] + extractedIns.Immediate + 1] = static_cast<uint8_t>(regs[extractedIns.Register2] >> 8);
                break;
            case Func3Type::SW:
                ram[regs[extractedIns.Register1] + extractedIns.Immediate] = static_cast<uint8_t>(regs[extractedIns.Register2]);
                ram[regs[extractedIns.Register1] + extractedIns.Immediate + 1] = static_cast<uint8_t>(regs[extractedIns.Register2] >> 8);
                ram[regs[extractedIns.Register1] + extractedIns.Immediate + 2] = static_cast<uint8_t>(regs[extractedIns.Register2] >> 16);
                ram[regs[extractedIns.Register1] + extractedIns.Immediate + 3] = static_cast<uint8_t>(regs[extractedIns.Register2] >> 24);
                break;
            }
            
            return 4;

        default:
            return 0;
        }
    }

    uint32_t SingleCycle::Cycle(std::array<uint8_t, 1024> &ram, RegisterContainer &regs, const RegisterInstruction &extractedIns)
    {
        switch (extractedIns.LAType)
        {
            case RegisterLAType::SLLI: // SLLI/SLL pois possuem a mesma combinação
                if (InstructionType::REGISTER_SHIFT == extractedIns.Opcode)
                    regs[extractedIns.RegisterDest] = regs[extractedIns.Register1] << extractedIns.Register2;
                else
                    regs[extractedIns.RegisterDest] = regs[extractedIns.Register1] << regs[extractedIns.Register2];
                break;
            case RegisterLAType::SRLI: // SRLI/SRL pois possuem a mesma combinação
                if (InstructionType::REGISTER_SHIFT == extractedIns.Opcode)
                    regs[extractedIns.RegisterDest] = regs[extractedIns.Register1] >> extractedIns.Register2;
                else
                    regs[extractedIns.RegisterDest] = regs[extractedIns.Register1] >> regs[extractedIns.Register2];
                break;
            case RegisterLAType::SRAI: // SRAI/SRA pois possuem a mesma combinação
                if (InstructionType::REGISTER_SHIFT == extractedIns.Opcode)
                    regs[extractedIns.RegisterDest] = static_cast<int32_t>(regs[extractedIns.Register1]) >> extractedIns.Register2;
                else
                    regs[extractedIns.RegisterDest] = static_cast<int32_t>(regs[extractedIns.Register1]) >> regs[extractedIns.Register2];
                break;
            case RegisterLAType::ADD:
                regs[extractedIns.RegisterDest] = regs[extractedIns.Register1] + regs[extractedIns.Register2];
                break;
            case RegisterLAType::SUB:
                regs[extractedIns.RegisterDest] = regs[extractedIns.Register1] - regs[extractedIns.Register2];
                break;    
            case RegisterLAType::SLT:
                regs[extractedIns.RegisterDest] = static_cast<int32_t>(regs[extractedIns.Register1]) < static_cast<int32_t>(regs[extractedIns.Register2]);
                break;    
            case RegisterLAType::SLTU:
                regs[extractedIns.RegisterDest] = regs[extractedIns.Register1] < regs[extractedIns.Register2];
                break;    
            case RegisterLAType::XOR:
                regs[extractedIns.RegisterDest] = regs[extractedIns.Register1] ^ regs[extractedIns.Register2];
                break;    
            case RegisterLAType::OR:
                regs[extractedIns.RegisterDest] = regs[extractedIns.Register1] | regs[extractedIns.Register2];
                break;    
            case RegisterLAType::AND:
                regs[extractedIns.RegisterDest] = regs[extractedIns.Register1] & regs[extractedIns.Register2];
                break;    
        }
        return 4;
    }

    uint32_t SingleCycle::Cycle(std::array<uint8_t, 1024> &ram, RegisterContainer &regs, uint32_t PC, const ImmediateInstruction &extractedIns)
    {
        if (static_cast<InstructionType>(extractedIns.Opcode) == InstructionType::IMMEDIATE_JALR)
        {
            regs[extractedIns.RegisterDest] = PC + 4;
            return extractedIns.Register1 + extractedIns.Immediate;
        }


        switch (extractedIns.LAType)
        {
        case ImmediateLAType::LB:
            regs[extractedIns.RegisterDest] = static_cast<int32_t>(static_cast<int8_t>(ram[regs[extractedIns.Register1] + extractedIns.Immediate]));
            break;
        case ImmediateLAType::LH:
            regs[extractedIns.RegisterDest] = static_cast<int32_t>(static_cast<int16_t>(ram[regs[extractedIns.Register1] + extractedIns.Immediate] | 
                                            ram[regs[extractedIns.Register1] + extractedIns.Immediate + 1] << 8));
            break;
        case ImmediateLAType::LW:
            regs[extractedIns.RegisterDest] = ram[regs[extractedIns.Register1] + extractedIns.Immediate] | 
                                            ram[regs[extractedIns.Register1] + extractedIns.Immediate + 1] << 8 |
                                            ram[regs[extractedIns.Register1] + extractedIns.Immediate + 2] << 16 |
                                            ram[regs[extractedIns.Register1] + extractedIns.Immediate + 3] << 24;
            break;
        case ImmediateLAType::LBU:
            regs[extractedIns.RegisterDest] = ram[regs[extractedIns.Register1] + extractedIns.Immediate];
            break;
        case ImmediateLAType::LHU:
            regs[extractedIns.RegisterDest] = ram[regs[extractedIns.Register1] + extractedIns.Immediate] | 
                                            ram[regs[extractedIns.Register1] + extractedIns.Immediate + 1] << 8;
            break;
        case ImmediateLAType::ADDI:
            regs[extractedIns.RegisterDest] = regs[extractedIns.Register1] + extractedIns.Immediate;
            break;
        case ImmediateLAType::SLTI:
            break;
        case ImmediateLAType::SLTIU:
            break;
        case ImmediateLAType::XORI:
            regs[extractedIns.RegisterDest] = regs[extractedIns.Register1] ^ extractedIns.Immediate;
            break;
        case ImmediateLAType::ORI:
            regs[extractedIns.RegisterDest] = regs[extractedIns.Register1] | extractedIns.Immediate;
            break;
        case ImmediateLAType::ANDI:
            regs[extractedIns.RegisterDest] = regs[extractedIns.Register1] & extractedIns.Immediate;
            break;
        default:
            break;
        }
        return 4;
    }

    uint32_t SingleCycle::Cycle(std::array<uint8_t, 1024> &ram, RegisterContainer &regs, uint32_t PC, const UpperImmediateInstruction &extractedIns)
    {
        switch (static_cast<InstructionType>(extractedIns.Opcode))
        {
        case InstructionType::UPPER_IMMEDIATE_LUI:
            regs[extractedIns.RegisterDest] = extractedIns.Immediate << 12;
            break;
        case InstructionType::UPPER_IMMEDIATE_AUI:
            regs[extractedIns.RegisterDest] = PC + extractedIns.Immediate << 12;
            break;
        }

        return 4;
    }

    uint32_t SingleCycle::Cycle(std::array<uint8_t, 1024> &ram, RegisterContainer &regs, uint32_t PC, uint8_t regDest, uint32_t immediate)
    {
        regs[regDest] = PC + 4;
        return immediate;
    }
}