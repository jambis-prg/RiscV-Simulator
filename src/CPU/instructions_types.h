#pragma once

#include <stdint.h>

namespace RSCV::CPU
{
    enum class Func3Type : uint8_t
    {
        BEQ     = 0b000,
        BNE     = 0b001,
        BLT     = 0b100,
        BGE     = 0b101,
        BLTU    = 0b110,
        BGEU    = 0b111,
        SB      = BEQ,
        SH      = BNE,
        SW      = 0b010,
    };

    enum class RegisterLAType : uint8_t
    {
        SLLI    = 0b00000001,
        SRLI    = 0b00000101,
        SRAI    = 0b01000101,
        ADD     = 0b00000000,
        SUB     = 0b01000000,
        SLL     = SLLI,
        SLT     = 0b00000010,
        SLTU    = 0b00000011,
        XOR     = 0b00000100,
        SRL     = SRLI,
        SRA     = SRAI,
        OR      = 0b00000110,
        AND     = 0b00000111
    };

    enum class ImmediateLAType : uint16_t
    {
        LB          = 0b00000000011,
        LH          = 0b00100000011,
        LW          = 0b01000000011,
        LBU         = 0b10000000011,
        LHU         = 0b10100000011,
        ADDI        = 0b00000010011,
        SLTI        = 0b01000010011,
        SLTIU       = 0b01100010011,
        XORI        = 0b10000010011,
        ORI         = 0b11000010011,
        ANDI        = 0b11100010011 
    };

    enum class InstructionType : uint8_t
    {
        BRANCH              = 0b01100011,
        STORE               = 0b00100011,
        REGISTER            = 0b00110011,
        // Instrucoes de SHIFT nao possui reg2 mas possui um shamt, mas posso aproveitar o tipo reg para extrair ele, e mudar a forma
        // de interpretrar na execução da operação
        REGISTER_SHIFT      = 0b00010011,
        IMMEDIATE           = 0b00000011,
        IMMEDIATE_ALU       = 0b00010011, // OBS: Possui o mesmo valor de REGISTER_SHIFT
        IMMEDIATE_JALR      = 0b01100111,
        UPPER_IMMEDIATE_LUI = 0b00110111,
        UPPER_IMMEDIATE_AUI = 0b00010111,
        JAL                 = 0b01101111
    };

    inline constexpr bool operator==(InstructionType lhs, InstructionType rhs) 
    {
        return static_cast<uint8_t>(lhs) == static_cast<uint8_t>(rhs);
    }

    inline constexpr bool operator==(InstructionType lhs, int rhs) 
    {
        return static_cast<uint8_t>(lhs) == static_cast<uint8_t>(rhs);
    }

    inline constexpr bool operator!=(InstructionType lhs, InstructionType rhs) 
    {
        return static_cast<uint8_t>(lhs) != static_cast<uint8_t>(rhs);
    }

    inline constexpr bool operator!=(InstructionType lhs, int rhs) 
    {
        return static_cast<uint8_t>(lhs) != static_cast<uint8_t>(rhs);
    }

    inline constexpr InstructionType operator|(InstructionType lhs, InstructionType rhs) 
    {
        return static_cast<InstructionType>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
    }

    inline constexpr InstructionType operator&(InstructionType lhs, InstructionType rhs) 
    {
        return static_cast<InstructionType>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
    }

    inline constexpr InstructionType operator^(InstructionType lhs, InstructionType rhs) 
    {
        return static_cast<InstructionType>(static_cast<uint8_t>(lhs) ^ static_cast<uint8_t>(rhs));
    }

    inline constexpr InstructionType operator~(InstructionType lhs) 
    {
        return static_cast<InstructionType>(~static_cast<uint8_t>(lhs));
    }

    // Ambos possuem os mesms campos, porém a forma como são extraidos é diferente
    struct StoreBranchInstruction
    {
        uint8_t Opcode;
        uint8_t Register1, Register2;
        Func3Type Func3;
        uint16_t Immediate;

        StoreBranchInstruction(uint8_t opcode, uint8_t reg1, uint8_t reg2, Func3Type func3, uint16_t immediate) :
        Opcode(opcode), Register1(reg1), Register2(reg2), Func3(func3), Immediate(immediate) {}
    };

    struct RegisterInstruction
    {
        uint8_t Opcode;
        uint8_t RegisterDest, Register1, Register2;
        RegisterLAType LAType;

        RegisterInstruction(uint8_t opcode, uint8_t regDest, uint8_t reg1, uint8_t reg2, uint8_t func3, uint8_t func7) :
        Opcode(opcode), RegisterDest(regDest), Register1(reg1), Register2(reg2), LAType(static_cast<RegisterLAType>(func7 | func3)) {}
    };

    struct ImmediateInstruction
    {
        uint8_t Opcode;
        uint8_t RegisterDest, Register1;
        ImmediateLAType LAType;
        uint16_t Immediate;

        ImmediateInstruction(uint8_t opcode, uint8_t regDest, uint8_t reg1, uint16_t immediate, uint8_t func3) :
        Opcode(opcode), RegisterDest(regDest), Register1(reg1), Immediate(immediate), LAType(static_cast<ImmediateLAType>(func3 << 8 | opcode)) {}
    };

    struct UpperImmediateInstruction
    {
        uint8_t Opcode;
        uint8_t RegisterDest;
        uint32_t Immediate;
        UpperImmediateInstruction(uint8_t opcode, uint8_t regDest, uint32_t immediate) :
        Opcode(opcode), RegisterDest(regDest), Immediate(immediate) {}
    };
    
}