#include "cpu.h"

namespace RSCV::CPU
{
    void CPU::Cycle(std::array<uint8_t, 1024> &ram)
    {
        const uint32_t instruction = ram[m_PC] | ram[m_PC + 1] << 8 | ram[m_PC + 2] << 16 | ram[m_PC + 3] << 24;
        const InstructionType opcode = static_cast<InstructionType>(instruction & 0x000000EF);
        
        switch (opcode)
        {
        case InstructionType::IMMEDIATE:
        case InstructionType::IMMEDIATE_ALU:
        case InstructionType::IMMEDIATE_JALR:
        {
            const uint8_t func3 = (instruction >> 12) & 0x07;
            // Trata caso especial de SLLI, SRLI e SRAI
            if (InstructionType::IMMEDIATE_ALU == opcode && (func3 == 0b001 || func3 == 0b101)) 
                goto REGISTER_SHIFT_LABEL;

            const uint8_t regDest = (instruction >> 7) & 0x1F;
            const uint8_t reg1 = (instruction >> 15) & 0x1F;
            const uint16_t immediate = (instruction >> 20) & 0x0FFF;
            
            ImmediateInstruction extractedtIns(static_cast<uint8_t>(opcode), regDest, reg1, immediate, func3);
            m_PC += m_PipelineRef.Cycle(ram, m_Regs, m_PC, extractedtIns);
        }
            break;
        case InstructionType::UPPER_IMMEDIATE_LUI:
        case InstructionType::UPPER_IMMEDIATE_AUI:
        {
            const uint8_t regDest = (instruction >> 7) & 0x1F;
            const uint32_t immediate = (instruction >> 12) & 0x0FFFFF;
            UpperImmediateInstruction extractedtIns(static_cast<uint8_t>(opcode), regDest, immediate);
            m_PC += m_PipelineRef.Cycle(ram, m_Regs, m_PC, extractedtIns);
        }
            break;
        case InstructionType::JAL:
        {
            const uint8_t regDest = (instruction >> 7) & 0x1F;
            const uint32_t immediate = (instruction >> 11) & 0x00100000 | (instruction >> 20) & 0x000007FE | (instruction >> 9) & 0x00000800 | instruction & 0x000FE0000;
            m_PC += m_PipelineRef.Cycle(ram, m_Regs, m_PC, regDest, immediate);
        }
            break;
        case InstructionType::BRANCH:
        case InstructionType::STORE:
        case InstructionType::REGISTER:
        // case InstructionType::REGISTER_SHIFT:
        {
            REGISTER_SHIFT_LABEL:
            const uint8_t reg1 = (instruction >> 15) & 0x1F;
            const uint8_t reg2 = (instruction >> 20) & 0x1F;
            const uint8_t func3 = (instruction >> 12) & 0x07;

            if ((opcode & (InstructionType::REGISTER | InstructionType::REGISTER_SHIFT)) != 0)
            {
                RegisterInstruction extractedIns(static_cast<uint8_t>(opcode), (instruction >> 7) & 0x1F, reg1, reg2, func3, (instruction >> 25) & 0x7F);
                m_PC += m_PipelineRef.Cycle(ram, m_Regs, extractedIns);
            }
            else if ((opcode & InstructionType::BRANCH) != 0)
            {
                const uint16_t immediate = (instruction >> 8) & 0x0F | (instruction << 4) & (0x01 << 11) | (instruction >> 19) & (0x01 << 12) | (instruction >> 20) & (0x3F << 5);
                StoreBranchInstruction extractedIns(static_cast<uint8_t>(opcode), reg1, reg2, static_cast<Func3Type>(func3), immediate);
                m_PC += m_PipelineRef.Cycle(ram, m_Regs, extractedIns);
            }
            else
            {
                StoreBranchInstruction extractedIns(static_cast<uint8_t>(opcode), reg1, reg2, static_cast<Func3Type>(func3), (instruction >> 7) & 0x1F | (instruction >> 20) & 0xEF);
                m_PC += m_PipelineRef.Cycle(ram, m_Regs, extractedIns);
            }
        }
            break;
        default:
            break;
        }
    }

    void CPU::Reset()
    {
        m_PipelineRef.Reset();
        m_Regs.Reset();
        m_PC = 0;
    }
}