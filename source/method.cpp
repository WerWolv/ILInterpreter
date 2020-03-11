#include "method.hpp"

#include <string>

#include "types.hpp"
#include "tables.hpp"
#include "dll.hpp"
#include "opcode.hpp"
#include "context.hpp"
#include "logger.hpp"

namespace ili  {

    Method::Method(Context &ctx, u32 methodToken) : m_ctx(ctx) {
        this->m_methodDef = this->m_ctx.dll->getMethodDefByToken(methodToken);
        Logger::debug("Executing method '%s'", this->m_ctx.dll->getString(this->m_methodDef->nameIndex));
    }

    void Method::execute() {
        section_table_entry_t *ilHeaderSection = this->m_ctx.dll->getVirtualSection(this->m_methodDef->rva);
        u8 *methodHeader = OFFSET(this->m_ctx.dll->getData(), VRA_TO_OFFSET(ilHeaderSection, this->m_methodDef->rva));

        if ((*methodHeader & 0x03) == 0x02) // Tiny Header
            this->m_programCounter = methodHeader + 1;
        else if ((*methodHeader & 0x03) == 0x03) // Fat Header
            this->m_programCounter = methodHeader + 12;

        for (u16 i = 0; i < 0xFF; i++)
            this->m_localVariable[i] = nullptr;

        u8 *methodStart = this->m_programCounter;

        while (true) {
            u8 currOpcode = *this->m_programCounter;

            this->m_programCounter++;

            if (currOpcode != 0xFE) { // Handle normal opcodes
                switch (static_cast<OpcodePrefix>(currOpcode)) {
                    case OpcodePrefix::Nop:
                        Logger::debug("Instruction NOP");
                        break;
                    case OpcodePrefix::Brk:
                        Logger::debug("Instruction BREAK");
                        raise(SIGILL);
                        break;
                    case OpcodePrefix::Call:
                    {
                        Logger::debug("Instruction CALL");
                        u32 token = this->getNext<u32>();
                        switch (TABLE_ID(token)) {
                            case TABLE_ID_METHODDEF:
                            {
                                auto calledMethod = new Method(this->m_ctx, token);
                                calledMethod->execute();
                                delete calledMethod;
                                break;
                            }
                            case TABLE_ID_MEMBERREF:
                            {
                                auto sig = this->m_ctx.dll->getMethodSignature(token);
                                Logger::debug("Executing native method %s", sig.c_str());

                                this->m_ctx.nativeFunctions[sig]();

                                break;
                            }
                        }
                        break;
                    }
                        break;
                    case OpcodePrefix::Stloc_0:
                        Logger::debug("Instruction STLOC.0");
                        stloc(0);
                        break;
                    case OpcodePrefix::Stloc_1:
                        Logger::debug("Instruction STLOC.1");
                        stloc(1);
                        break;
                    case OpcodePrefix::Stloc_2:
                        Logger::debug("Instruction STLOC.2");
                        stloc(2);
                        break;
                    case OpcodePrefix::Stloc_3:
                        Logger::debug("Instruction STLOC.3");
                        stloc(3);
                        break;
                    case OpcodePrefix::Stloc_s:
                        Logger::debug("Instruction STLOC.s");
                        stloc(getNext<u8>());
                        break;
                    case OpcodePrefix::Ldc_i4_0:
                        Logger::debug("Instruction LDC.I4.0");
                        ldc<s32>(Type::Int32, 0);
                        break;
                    case OpcodePrefix::Ldc_i4_1:
                        Logger::debug("Instruction LDC.I4.1");
                        ldc<s32>(Type::Int32, 1);
                        break;
                    case OpcodePrefix::Ldc_i4_2:
                        Logger::debug("Instruction LDC.I4.2");
                        ldc<s32>(Type::Int32, 2);
                        break;
                    case OpcodePrefix::Ldc_i4_3:
                        Logger::debug("Instruction LDC.I4.3");
                        ldc<s32>(Type::Int32, 3);
                        break;
                    case OpcodePrefix::Ldc_i4_4:
                        Logger::debug("Instruction LDC.I4.4");
                        ldc<s32>(Type::Int32, 4);
                        break;
                    case OpcodePrefix::Ldc_i4_5:
                        Logger::debug("Instruction LDC.I4.5");
                        ldc<s32>(Type::Int32, 5);
                        break;
                    case OpcodePrefix::Ldc_i4_6:
                        Logger::debug("Instruction LDC.I4.6");
                        ldc<s32>(Type::Int32, 6);
                        break;
                    case OpcodePrefix::Ldc_i4_7:
                        Logger::debug("Instruction LDC.I4.7");
                        ldc<s32>(Type::Int32, 7);
                        break;
                    case OpcodePrefix::Ldc_i4_8:
                        Logger::debug("Instruction LDC.I4.8");
                        ldc<s32>(Type::Int32, 8);
                        break;
                    case OpcodePrefix::Ldc_i4_m1:
                        Logger::debug("Instruction LDC.I4.M1");
                        ldc<s32>(Type::Int32, -1);
                        break;
                    case OpcodePrefix::Ldc_i4:
                        Logger::debug("Instruction LDC.I4");
                        ldc<s32>(Type::Int32, getNext<s32>());
                        break;
                    case OpcodePrefix::Ldc_i8:
                        Logger::debug("Instruction LDC.I8");
                        ldc<s64>(Type::Int64, getNext<s64>());
                        break;
                    case OpcodePrefix::Ldc_r4:
                        Logger::debug("Instruction LDC.R4");
                        ldc<double>(Type::F, getNext<float>());
                        break;
                    case OpcodePrefix::Ldc_r8:
                        Logger::debug("Instruction LDC.R8");
                        ldc<double>(Type::F, getNext<double>());
                        break;
                    case OpcodePrefix::Ldc_i4_s:
                        Logger::debug("Instruction LDC.I4.s");
                        ldc<s32>(Type::Int32, getNext<s8>());
                        break;
                    case OpcodePrefix::Ldloc_0:
                        Logger::debug("Instruction LDLOC.0");
                        ldloc(0);
                        break;
                    case OpcodePrefix::Ldloc_1:
                        Logger::debug("Instruction LDLOC.1");
                        ldloc(1);
                        break;
                    case OpcodePrefix::Ldloc_2:
                        Logger::debug("Instruction LDLOC.2");
                        ldloc(2);
                        break;
                    case OpcodePrefix::Ldloc_3:
                        Logger::debug("Instruction LDLOC.3");
                        ldloc(3);
                        break;
                    case OpcodePrefix::Ldloc_s:
                        Logger::debug("Instruction LDLOC.s");
                        ldloc(getNext<u8>());
                        break;
                    case OpcodePrefix::Ldstr:
                        Logger::debug("Instruction LDSTR");
                        this->m_ctx.push<u32>(Type::O, getNext<u32>());
                        break;
                    case OpcodePrefix::Br:
                        Logger::debug("Instruction BR");
                        this->m_programCounter = methodStart + getNext<s32>();
                        break;
                    case OpcodePrefix::Br_s:
                        Logger::debug("Instruction BR.S");
                        this->m_programCounter += getNext<s8>();
                        break;
                    case OpcodePrefix::Ret:
                        Logger::debug("Instruction RET");
                        for (u16 i = 0; i < 0xFF; i++) {
                            if (this->m_localVariable[i] = nullptr)
                                continue;

                            delete this->m_localVariable[i];
                            this->m_localVariable[i] = nullptr;
                        }
                        return;
                    default:
                        Logger::error("Unknown opcode (%x)!", currOpcode);
                        exit(1);
                        break;
                }
            }
            else { // Handle extended opcodes
                currOpcode = *this->m_programCounter;
                this->m_programCounter++;

                switch (currOpcode) {

                }
            }
        }
    }

    // General Operations

    template<typename T>
    T Method::getNext() {
        T value = *reinterpret_cast<T*>(this->m_programCounter);
        this->m_programCounter += sizeof(T);

        return value;
    }

    // Instruction Implementations

    void Method::stloc(u8 id) {
        if (this->m_localVariable[id] != nullptr)
            delete this->m_localVariable[id];
        this->m_localVariable[id] = nullptr;

        Type type = this->m_ctx.getTypeOnStack();

        switch (type) {
            case Type::Int32:
                this->m_localVariable[id] = new Variable<s32>{type, this->m_ctx.pop<s32>()};
                break;
            case Type::Int64:
                this->m_localVariable[id] = new Variable<s64>{type, this->m_ctx.pop<s64>()};
                break;
            case Type::Native_int:
                this->m_localVariable[id] = new Variable<s32>{type, this->m_ctx.pop<s32>()};
                break;
            case Type::F:
                this->m_localVariable[id] = new Variable<double>{type, this->m_ctx.pop<double>()};
                break;
            case Type::O:
                this->m_localVariable[id] = new Variable<u32>{type, this->m_ctx.pop<u32>()};
                break;
            case Type::Native_unsigned_int:
                this->m_localVariable[id] = new Variable<u32>{type, this->m_ctx.pop<u32>()};
                break;
            case Type::Pointer:
                this->m_localVariable[id] = new Variable<u64>{type, this->m_ctx.pop<u64>()};
                break;
        }
    }

    void Method::ldloc(u8 id) {
        auto varType = this->m_localVariable[id]->type;

        switch (varType) {
            case Type::Int32:
                this->m_ctx.push<s32>(varType, static_cast<Variable<s32>*>(this->m_localVariable[id])->value);
                break;
            case Type::Int64:
                this->m_ctx.push<s64>(varType, static_cast<Variable<s64>*>(this->m_localVariable[id])->value);
                break;
            case Type::Native_int:
                this->m_ctx.push<s32>(varType, static_cast<Variable<s32>*>(this->m_localVariable[id])->value);
                break;
            case Type::F:
                this->m_ctx.push<double>(varType, static_cast<Variable<double>*>(this->m_localVariable[id])->value);
                break;
            case Type::O:
                this->m_ctx.push<u32>(varType, static_cast<Variable<u32>*>(this->m_localVariable[id])->value);
                break;
            case Type::Native_unsigned_int:
                this->m_ctx.push<u32>(varType, static_cast<Variable<u32>*>(this->m_localVariable[id])->value);
                break;
            case Type::Pointer:
                this->m_ctx.push<u64>(varType, static_cast<Variable<u64>*>(this->m_localVariable[id])->value);
                break;
        }
    }

    template<typename T>
    void Method::ldc(Type type, T num) {
        Variable<T> var{ type, num };

        this->m_ctx.stackPointer += sizeof(T);

        std::memcpy(this->m_ctx.stackPointer, &var, sizeof(T));
    }

}

