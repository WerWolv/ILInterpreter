#pragma once

#include <ili/types.hpp>

namespace ili::op {

    enum class Opcode : u16 {
        Nop         = 0x00,
        Brk         = 0x01,
        Ldarg_0     = 0x02,
        Ldarg_1     = 0x03,
        Ldarg_2     = 0x04,
        Ldarg_3     = 0x05,
        Ldloc_0     = 0x06,
        Ldloc_1     = 0x07,
        Ldloc_2     = 0x08,
        Ldloc_3     = 0x09,
        Stloc_0     = 0x0A,
        Stloc_1     = 0x0B,
        Stloc_2     = 0x0C,
        Stloc_3     = 0x0D,
        Ldarg_s     = 0x0E,
        Ldarga_s    = 0x0F,
        Starg_s     = 0x10,
        Ldloc_s     = 0x11,
        Ldloca_s    = 0x12,
        Stloc_s     = 0x13,
        Ldnull      = 0x14,
        Ldc_i4_m1   = 0x15,
        Ldc_i4_0    = 0x16,
        Ldc_i4_1    = 0x17,
        Ldc_i4_2    = 0x18,
        Ldc_i4_3    = 0x19,
        Ldc_i4_4    = 0x1A,
        Ldc_i4_5    = 0x1B,
        Ldc_i4_6    = 0x1C,
        Ldc_i4_7    = 0x1D,
        Ldc_i4_8    = 0x1E,
        Ldc_i4_s    = 0x1F,
        Ldc_i4      = 0x20,
        Ldc_i8      = 0x21,
        Ldc_r4      = 0x22,
        Ldc_r8      = 0x23,
        Dup         = 0x25,
        Pop         = 0x26,
        Jmp         = 0x27,
        Call        = 0x28,
        Calli       = 0x29,
        Ret         = 0x2A,
        Br_s        = 0x2B,
        Brfalse_s,
        Brtrue_s,
        Beq_s,
        Bge_s,
        Bgt_s,
        Ble_s,
        Blt_s,
        Bne_un_s,
        Bge_un_s,
        Bgt_un_s,
        Ble_un_s,
        Blt_un_s,
        Br          = 0x38,
        Brfalse,
        Brtrue,
        Beq,
        Bge,
        Bgt,
        Ble,
        Blt,
        Bne_un,
        Bge_un,
        Bgt_un,
        Ble_un,
        Blt_un,
        Swtch,
        Ldind_i1,
        Ldind_u1,
        Ldind_i2,
        Ldind_u2,
        Ldind_i4,
        Ldind_u4,
        Ldind_i8,
        Ldind_i,
        Ldind_r4,
        Ldind_r8,
        Ldind_ref,
        Stind_ref,
        Stind_i1,
        Stind_i2,
        Stind_i4,
        Stind_i8,
        Stind_r4,
        Stind_r8,
        Add,
        Sub,
        Mul,
        Div,
        Div_un,
        Rem,
        Rem_un,
        Bitwise_and,
        Bitwise_or,
        Bitwise_xor,
        Shl,
        Shr,
        Shr_un,
        Neg,
        Bitwise_not,
        Conv_i1,
        Conv_i2,
        Conv_i4,
        Conv_i8,
        Conv_r4,
        Conv_r8,
        Conv_u4,
        Conv_u8,
        Callvirt,
        Cpobj,
        Ldobj,
        Ldstr = 0x72,
        Newobj,
        Castclass,
        Isinst,
        Conv_r_un = 0x76,
        Unbox = 0x79,
        Thrw,
        Ldfld,
        Ldflda,
        Stfld,
        Ldsfld,
        Ldsflda,
        Stsfld,
        Stobj,
        Conv_ovf_i1_un,
        Conv_ovf_i2_un,
        Conv_ovf_i4_un,
        Conv_ovf_i8_un,
        Conv_ovf_u1_un,
        Conv_ovf_u2_un,
        Conv_ovf_u4_un,
        Conv_ovf_u8_un,
        Conv_ovf_i_un,
        Conv_ovf_u_un,
        Box,
        Newarr,
        Ldlen,
        Ldelema,
        Ldelem_i1,
        Ldelem_u1,
        Ldelem_i2,
        Ldelem_u2,
        Ldelem_i4,
        Ldelem_u4,
        Ldelem_i8,
        Ldelem_i,
        Ldelem_r4,
        Ldelem_r8,
        Ldelem_ref,
        Stelem_i,
        Stelem_i1,
        Stelem_i2,
        Stelem_i4,
        Stelem_i8,
        Stelem_r4,
        Stelem_r8,
        Stelem_ref,
        Ldelem,
        Stelem,
        Unbox_any = 0xA5,
        Conv_ovf_i1 = 0xB3,
        Conv_ovf_u1,
        Conv_ovf_i2,
        Conv_ovf_u2,
        Conv_ovf_i4,
        Conv_ovf_u4,
        Conv_ovf_i8,
        Conv_ovf_u8 = 0xBA,
        Refanyval = 0xC2,
        Ckfinite = 0xC3,
        Mkrefany = 0xC6,
        Ldtoken = 0xD0,
        Conv_u2,
        Conv_u1,
        Conv_i,
        Conv_ovf_i,
        Conv_ovf_u,
        Add_ovf,
        Add_ovf_un,
        Mul_ovf,
        Mul_ovf_un,
        Sub_ovf,
        Sub_ovf_un,
        Endfinally,
        Leave,
        Leave_s,
        Stind_i,
        Conv_u,

        Extended = 0xFE,
        Arglist = 0xFE00,
        Ceq,
        Cgt,
        Cgt_un,
        Clt,
        Clt_un,
        Ldftn,
        Ldvirtftn = 0xFE07,
        Ldarg = 0xFE09,
        Ldarga = 0xFE0A,
        Starg,
        Ldloc,
        Ldloca,
        Stloc,
        Localloc = 0xFE0F,
        Endfilter = 0xFE11,
        Unaligned,
        Volatle,
        Tail,
        Initobj,
        Constrained,
        Cpblk,
        Initblk,
        No,
        Rethrow = 0xFE1A,
        Size_of = 0xFE1C,
        Refanytype,
        Readonly
    };

    class Instruction {
    public:
        explicit Instruction(std::span<const u8> bytes) {
            m_bytes = bytes;
            m_bytes = m_bytes.subspan(0, getLength());
        }

        [[nodiscard]] Opcode getOpcode() const {
            auto opcode = Opcode(m_bytes[0]);
            if (opcode == Opcode::Extended) [[unlikely]] {
                opcode = Opcode(m_bytes[0] << 8 | m_bytes[1]);
            }

            return opcode;
        }

        [[nodiscard]] std::span<const u8> getArguments() const { return m_bytes.subspan(Opcode(m_bytes[0]) == Opcode::Extended ? 2 : 1); }

        template<typename T>
        [[nodiscard]]
        T get(u32 offset) const {
            auto result = T();

            std::memmove(&result, getArguments().data() + offset, sizeof(T));

            return result;
        }

        [[nodiscard]] std::size_t getLength() const {
            constexpr static u8 Opcode = 1;
            constexpr static u8 ExtendedOpcode = 2;
            constexpr static u8 Token = 4;
            constexpr static u8 uint8 = 1;
            constexpr static u8 uint16 = 2;
            constexpr static u8 uint32 = 4;
            constexpr static u8 int8 = 1;
            constexpr static u8 int32 = 4;
            constexpr static u8 int64 = 8;
            constexpr static u8 float32 = 4;
            constexpr static u8 float64 = 8;

            using enum ili::op::Opcode;
            switch (getOpcode()) {
                case Add:               return Opcode;
                case Add_ovf:           return Opcode;
                case Add_ovf_un:        return Opcode;
                case Bitwise_and:       return Opcode;
                case Arglist:           return ExtendedOpcode;
                case Beq:               return Opcode + int32;
                case Beq_s:             return Opcode + int8;
                case Bge:               return Opcode + int32;
                case Bge_s:             return Opcode + int8;
                case Bge_un:            return Opcode + int32;
                case Bge_un_s:          return Opcode + int8;
                case Bgt:               return Opcode + int32;
                case Bgt_s:             return Opcode + int8;
                case Bgt_un:            return Opcode + int32;
                case Bgt_un_s:          return Opcode + int8;
                case Ble:               return Opcode + int32;
                case Ble_s:             return Opcode + int8;
                case Ble_un:            return Opcode + int32;
                case Ble_un_s:          return Opcode + int8;
                case Blt:               return Opcode + int32;
                case Blt_s:             return Opcode + int8;
                case Blt_un:            return Opcode + int32;
                case Blt_un_s:          return Opcode + int8;
                case Bne_un:            return Opcode + int32;
                case Bne_un_s:          return Opcode + int8;
                case Br:                return Opcode + int32;
                case Br_s:              return Opcode + int8;
                case Brk:               return Opcode;
                // case brnull:
                // case brzero:
                case Brfalse :          return Opcode + int32;
                // case brnull_s:
                // case brzero_s:
                case Brfalse_s:         return Opcode + int8;
                // case brinst:
                case Brtrue:            return Opcode + int32;
                // case brinst_s:
                case Brtrue_s:          return Opcode + int8;
                case Call:              return Opcode + Token;
                case Calli:             return Opcode + Token;
                case Ceq:               return ExtendedOpcode;
                case Cgt:               return ExtendedOpcode;
                case Cgt_un:            return ExtendedOpcode;
                case Ckfinite:          return Opcode;
                case Clt:               return ExtendedOpcode;
                case Clt_un:            return ExtendedOpcode;
                case Conv_i1:           return Opcode;
                case Conv_i2:           return Opcode;
                case Conv_i4:           return Opcode;
                case Conv_i8:           return Opcode;
                case Conv_r4:           return Opcode;
                case Conv_r8:           return Opcode;
                case Conv_u1:           return Opcode;
                case Conv_u2:           return Opcode;
                case Conv_u4:           return Opcode;
                case Conv_u8:           return Opcode;
                case Conv_i:            return Opcode;
                case Conv_u:            return Opcode;
                case Conv_r_un:         return Opcode;
                case Conv_ovf_i1:       return Opcode;
                case Conv_ovf_i2:       return Opcode;
                case Conv_ovf_i4:       return Opcode;
                case Conv_ovf_i8:       return Opcode;
                case Conv_ovf_u1:       return Opcode;
                case Conv_ovf_u2:       return Opcode;
                case Conv_ovf_u4:       return Opcode;
                case Conv_ovf_u8:       return Opcode;
                case Conv_ovf_i:        return Opcode;
                case Conv_ovf_u:        return Opcode;
                case Conv_ovf_i1_un:    return Opcode;
                case Conv_ovf_i2_un:    return Opcode;
                case Conv_ovf_i4_un:    return Opcode;
                case Conv_ovf_i8_un:    return Opcode;
                case Conv_ovf_u1_un:    return Opcode;
                case Conv_ovf_u2_un:    return Opcode;
                case Conv_ovf_u4_un:    return Opcode;
                case Conv_ovf_u8_un:    return Opcode;
                case Conv_ovf_i_un:     return Opcode;
                case Conv_ovf_u_un:     return Opcode;
                case Cpblk:             return Opcode;
                case Div:               return Opcode;
                case Div_un:            return Opcode;
                case Dup:               return Opcode;
                case Endfilter:         return ExtendedOpcode;
                // case Endfault:
                case Endfinally:        return Opcode;
                case Initblk:           return ExtendedOpcode;
                case Jmp:               return Opcode + Token;
                case Ldarg:             return ExtendedOpcode + uint16;
                case Ldarg_s:           return Opcode + uint8;
                case Ldarg_0:           return Opcode;
                case Ldarg_1:           return Opcode;
                case Ldarg_2:           return Opcode;
                case Ldarg_3:           return Opcode;
                case Ldarga:            return ExtendedOpcode + uint16;
                case Ldarga_s:          return Opcode + uint8;
                case Ldc_i4:            return Opcode + int32;
                case Ldc_i8:            return Opcode + int64;
                case Ldc_r4:            return Opcode + float32;
                case Ldc_r8:            return Opcode + float64;
                case Ldc_i4_0:          return Opcode;
                case Ldc_i4_1:          return Opcode;
                case Ldc_i4_2:          return Opcode;
                case Ldc_i4_3:          return Opcode;
                case Ldc_i4_4:          return Opcode;
                case Ldc_i4_5:          return Opcode;
                case Ldc_i4_6:          return Opcode;
                case Ldc_i4_7:          return Opcode;
                case Ldc_i4_8:          return Opcode;
                // case Ldc_i4_M1:
                case Ldc_i4_m1:         return Opcode;
                case Ldc_i4_s:          return Opcode + int8;
                case Ldftn:             return ExtendedOpcode + Token;
                case Ldind_i1:          return Opcode;
                case Ldind_i2:          return Opcode;
                case Ldind_i4:          return Opcode;
                case Ldind_i8:          return Opcode;
                case Ldind_u1:          return Opcode;
                case Ldind_u2:          return Opcode;
                case Ldind_u4:          return Opcode;
                case Ldind_r4:          return Opcode;
                case Ldind_r8:          return Opcode;
                case Ldind_i:           return Opcode;
                case Ldind_ref:         return Opcode;
                case Ldloc:             return ExtendedOpcode + uint16;
                case Ldloc_s:           return Opcode + uint8;
                case Ldloc_0:           return Opcode;
                case Ldloc_1:           return Opcode;
                case Ldloc_2:           return Opcode;
                case Ldloc_3:           return Opcode;
                case Ldloca:            return ExtendedOpcode + uint16;
                case Ldloca_s:          return Opcode + uint8;
                case Ldnull:            return Opcode;
                case Leave:             return Opcode + int32;
                case Leave_s:           return Opcode + int8;
                case Localloc:          return ExtendedOpcode;
                case Mul:               return Opcode;
                case Mul_ovf:           return Opcode;
                case Mul_ovf_un:        return Opcode;
                case Neg:               return Opcode;
                case Nop:               return Opcode;
                case Bitwise_not:       return Opcode;
                case Bitwise_or:        return Opcode;
                case Pop:               return Opcode;
                case Rem:               return Opcode;
                case Rem_un:            return Opcode;
                case Ret:               return Opcode;
                case Shl:               return Opcode;
                case Shr:               return Opcode;
                case Shr_un:            return Opcode;
                case Starg:             return ExtendedOpcode + uint16;
                case Starg_s:           return Opcode + uint8;
                case Stind_i1:          return Opcode;
                case Stind_i2:          return Opcode;
                case Stind_i4:          return Opcode;
                case Stind_i8:          return Opcode;
                case Stind_r4:          return Opcode;
                case Stind_r8:          return Opcode;
                case Stind_i:           return Opcode;
                case Stind_ref:         return Opcode;
                case Stloc:             return ExtendedOpcode + uint16;
                case Stloc_s:           return Opcode + uint8;
                case Stloc_0:           return Opcode;
                case Stloc_1:           return Opcode;
                case Stloc_2:           return Opcode;
                case Stloc_3:           return Opcode;
                case Sub:               return Opcode;
                case Sub_ovf:           return Opcode;
                case Sub_ovf_un:        return Opcode;
                case Swtch:             return Opcode + uint32 + uint32 * get<u32>(0);
                case Bitwise_xor:       return Opcode;
                case Box:               return Opcode + Token;
                case Callvirt:          return Opcode + Token;
                case Castclass:         return Opcode + Token;
                case Cpobj:             return Opcode + Token;
                case Initobj:           return Opcode + Token;
                case Isinst:            return Opcode + Token;
                case Ldelem:            return Opcode + Token;
                case Ldelem_i1:         return Opcode;
                case Ldelem_i2:         return Opcode;
                case Ldelem_i4:         return Opcode;
                case Ldelem_i8:         return Opcode;
                case Ldelem_u1:         return Opcode;
                case Ldelem_u2:         return Opcode;
                case Ldelem_u4:         return Opcode;
                case Ldelem_r4:         return Opcode;
                case Ldelem_r8:         return Opcode;
                case Ldelem_i:          return Opcode;
                case Ldelem_ref:        return Opcode;
                case Ldelema:           return Opcode + Token;
                case Ldfld:             return Opcode + Token;
                case Ldflda:            return Opcode + Token;
                case Ldlen:             return Opcode;
                case Ldobj:             return Opcode + Token;
                case Ldsfld:            return Opcode + Token;
                case Ldsflda:           return Opcode + Token;
                case Ldstr:             return Opcode + Token;
                case Ldtoken:           return Opcode + Token;
                case Ldvirtftn:         return ExtendedOpcode + Token;
                case Mkrefany:          return Opcode + Token;
                case Newarr:            return Opcode + Token;
                case Newobj:            return Opcode + Token;
                case Refanytype:        return ExtendedOpcode;
                case Refanyval:         return Opcode + Token;
                case Rethrow:           return ExtendedOpcode;
                case Size_of:           return ExtendedOpcode + Token;
                case Stelem:            return Opcode + Token;
                case Stelem_i1:         return Opcode;
                case Stelem_i2:         return Opcode;
                case Stelem_i4:         return Opcode;
                case Stelem_i8:         return Opcode;
                case Stelem_r4:         return Opcode;
                case Stelem_r8:         return Opcode;
                case Stelem_i:          return Opcode;
                case Stelem_ref:        return Opcode;
                case Stfld:             return Opcode + Token;
                case Stobj:             return Opcode + Token;
                case Stsfld:            return Opcode + Token;
                case Thrw:              return Opcode;
                case Unbox:             return Opcode + Token;
                case Unbox_any:         return Opcode + Token;
                default: return 0;
            }
        }

    private:
        std::span<const u8> m_bytes;
    };

}