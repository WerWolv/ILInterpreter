#pragma once

#include "types.hpp"

namespace ili {

    enum class OpcodePrefix : u16 {
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
        Logical_and,
        Logical_or,
        Logical_xor,
        Shl,
        Shr,
        Shr_un,
        Neg,
        Logical_not,
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

        Arglist = 0xFE00,
        Ceq,
        Cgt,
        Cgt_un,
        Clt,
        Clt_un,
        Ldftn,
        Ldvirtftn = 0xFE07,
        Ldarg = 0xFE0A,
        Ldarga,
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
}