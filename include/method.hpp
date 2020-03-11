#pragma once

#include <type_traits>

#include "types.hpp"
#include "tables.hpp"
#include "dll.hpp"
#include "opcode.hpp"
#include "context.hpp"

namespace ili  {

    class Method {
    public:
        Method(Context &context, u32 methodToken) : m_context(context) {
            this->m_methodDef = this->m_context.dll->getMethodByToken(methodToken);
            printf("Method Name: %s\n", this->m_context.dll->getString(this->m_methodDef->nameIndex));
        }

        void execute() {
            section_table_entry_t *ilHeaderSection = this->m_context.dll->getVirtualSection(this->m_methodDef->rva);
            u8 *methodHeader = OFFSET(this->m_context.dll->getData(), VRA_TO_OFFSET(ilHeaderSection, this->m_methodDef->rva));

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
                            printf("NOP\n");
                            break;
                        case OpcodePrefix::Brk:
                            printf("BREAK\n");
                            raise(SIGILL);
                            break;
                        case OpcodePrefix::Call:
                        {
                            printf("CALL\n");
                            u32 methodToken = this->getNext<u32>();
                            auto calledMethod = new Method(this->m_context, methodToken);
                            calledMethod->execute();
                            delete calledMethod;
                        }
                            break;
                        case OpcodePrefix::Stloc_0:
                            printf("STLOC.0\n");
                            stloc(0);
                            break;
                        case OpcodePrefix::Stloc_1:
                            printf("STLOC.1\n");
                            stloc(1);
                            break;
                        case OpcodePrefix::Stloc_2:
                            printf("STLOC.2\n");
                            stloc(2);
                            break;
                        case OpcodePrefix::Stloc_3:
                            printf("STLOC.3\n");
                            stloc(3);
                            break;
                        case OpcodePrefix::Stloc_s:
                            printf("STLOC.s\n");
                            stloc(getNext<u8>());
                            break;
                        case OpcodePrefix::Ldc_i4_0:
                            printf("LDC.I4.0\n");
                            ldc<s32>(Type::Int32, 0);
                            break;
                        case OpcodePrefix::Ldc_i4_1:
                            printf("LDC.I4.1\n");
                            ldc<s32>(Type::Int32, 1);
                            break;
                        case OpcodePrefix::Ldc_i4_2:
                            printf("LDC.I4.2\n");
                            ldc<s32>(Type::Int32, 2);
                            break;
                        case OpcodePrefix::Ldc_i4_3:
                            printf("LDC.I4.3\n");
                            ldc<s32>(Type::Int32, 3);
                            break;
                        case OpcodePrefix::Ldc_i4_4:
                            printf("LDC.I4.4\n");
                            ldc<s32>(Type::Int32, 4);
                            break;
                        case OpcodePrefix::Ldc_i4_5:
                            printf("LDC.I4.5\n");
                            ldc<s32>(Type::Int32, 5);
                            break;
                        case OpcodePrefix::Ldc_i4_6:
                            printf("LDC.I4.6\n");
                            ldc<s32>(Type::Int32, 6);
                            break;
                        case OpcodePrefix::Ldc_i4_7:
                            printf("LDC.I4.7\n");
                            ldc<s32>(Type::Int32, 7);
                            break;
                        case OpcodePrefix::Ldc_i4_8:
                            printf("LDC.I4.8\n");
                            ldc<s32>(Type::Int32, 8);
                            break;
                        case OpcodePrefix::Ldc_i4_m1:
                            printf("LDC.I4.M1\n");
                            ldc<s32>(Type::Int32, -1);
                            break;
                        case OpcodePrefix::Ldc_i4:
                            printf("LDC.I4\n");
                            ldc<s32>(Type::Int32, getNext<s32>());
                            break;
                        case OpcodePrefix::Ldc_i8:
                            printf("LDC.I8\n");
                            ldc<s64>(Type::Int64, getNext<s64>());
                            break;
                        case OpcodePrefix::Ldc_r4:
                            printf("LDC.R4\n");
                            ldc<double>(Type::F, getNext<float>());
                            break;
                        case OpcodePrefix::Ldc_r8:
                            printf("LDC.R8\n");
                            ldc<double>(Type::F, getNext<double>());
                            break;
                        case OpcodePrefix::Ldc_i4_s:
                            printf("LDC.I4.s\n");
                            ldc<s32>(Type::Int32, getNext<s8>());
                            break;
                        case OpcodePrefix::Ldloc_0:
                            printf("LDLOC.0\n");
                            ldloc(0);
                            break;
                        case OpcodePrefix::Ldloc_1:
                            printf("LDLOC.1\n");
                            ldloc(1);
                            break;
                        case OpcodePrefix::Ldloc_2:
                            printf("LDLOC.2\n");
                            ldloc(2);
                            break;
                        case OpcodePrefix::Ldloc_3:
                            printf("LDLOC.3\n");
                            ldloc(3);
                            break;
                        case OpcodePrefix::Ldloc_s:
                            printf("LDLOC.s\n");
                            ldloc(getNext<u8>());
                            break;
                        case OpcodePrefix::Ldstr:
                            printf("LDSTR\n");
                            this->push<u32>(Type::O, getNext<u32>());
                            break;
                        case OpcodePrefix::Br:
                            printf("BR\n");
                            this->m_programCounter = methodStart + getNext<s32>();
                            break;
                        case OpcodePrefix::Br_s:
                            printf("BR.S\n");
                            this->m_programCounter += getNext<s8>();
                            break;
                        case OpcodePrefix::Ret:
                            printf("RET\n");
                            for (u16 i = 0; i < 0xFF; i++) {
                                if (this->m_localVariable[i] = nullptr)
                                    continue;

                                delete this->m_localVariable[i];
                                this->m_localVariable[i] = nullptr;
                            }
                            return;
                        default:
                            printf("Unknown opcode (%x)!\n", currOpcode);
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

    private:
        Context &m_context;
        table_method_def_t *m_methodDef;

        u8 *m_programCounter;

        VariableBase *m_localVariable[0xFF] = { nullptr };
        VariableBase *m_returnVariable = nullptr;


        // General Operations

        template<typename T>
        T getNext() {
            T value = *reinterpret_cast<T*>(this->m_programCounter);
            this->m_programCounter += sizeof(T);

            return value;
        }

        Type getTypeOnStack() {
            return *(this->m_context.typeStackPointer - 1);
        }

        template<typename T>
        T pop() {
            this->m_context.typeStackPointer--;
            this->m_context.stackPointer -= sizeof(T);
            T ret;

            std::memset(&ret, 0x00, sizeof(T));
            std::memcpy(&ret, this->m_context.stackPointer, getTypeSize(getTypeOnStack()));

            return ret;
        }

        template<typename T>
        void push(Type type, T val) {

            std::memcpy(this->m_context.stackPointer, &val, getTypeSize(type));

            *this->m_context.typeStackPointer = type;
            this->m_context.typeStackPointer++;
            this->m_context.stackPointer += sizeof(T);
        }


        // Instruction Implementations

        void stloc(u8 id) {
            if (this->m_localVariable[id] != nullptr)
                delete this->m_localVariable[id];
            this->m_localVariable[id] = nullptr;

            Type type = getTypeOnStack();

            switch (type) {
                case Type::Int32:
                    this->m_localVariable[id] = new Variable<s32>{type, this->pop<s32>()};
                    break;
                case Type::Int64:
                    this->m_localVariable[id] = new Variable<s64>{type, this->pop<s64>()};
                    break;
                case Type::Native_int:
                    this->m_localVariable[id] = new Variable<s32>{type, this->pop<s32>()};
                    break;
                case Type::F:
                    this->m_localVariable[id] = new Variable<double>{type, this->pop<double>()};
                    break;
                case Type::O:
                    this->m_localVariable[id] = new Variable<u32>{type, this->pop<u32>()};
                    break;
                case Type::Native_unsigned_int:
                    this->m_localVariable[id] = new Variable<u32>{type, this->pop<u32>()};
                    break;
                case Type::Pointer:
                    this->m_localVariable[id] = new Variable<u64>{type, this->pop<u64>()};
                    break;
            }
        }

        void ldloc(u8 id) {
            auto varType = this->m_localVariable[id]->type;

            switch (varType) {
                case Type::Int32:
                    this->push<s32>(varType, static_cast<Variable<s32>*>(this->m_localVariable[id])->value);
                    break;
                case Type::Int64:
                    this->push<s64>(varType, static_cast<Variable<s64>*>(this->m_localVariable[id])->value);
                    break;
                case Type::Native_int:
                    this->push<s32>(varType, static_cast<Variable<s32>*>(this->m_localVariable[id])->value);
                    break;
                case Type::F:
                    this->push<double>(varType, static_cast<Variable<double>*>(this->m_localVariable[id])->value);
                    break;
                case Type::O:
                    this->push<u32>(varType, static_cast<Variable<u32>*>(this->m_localVariable[id])->value);
                    break;
                case Type::Native_unsigned_int:
                    this->push<u32>(varType, static_cast<Variable<u32>*>(this->m_localVariable[id])->value);
                    break;
                case Type::Pointer:
                    this->push<u64>(varType, static_cast<Variable<u64>*>(this->m_localVariable[id])->value);
                    break;
            }
        }

        template<typename T>
        void ldc(Type type, T num) {
            Variable<T> var{ type, num };

            this->m_context.stackPointer += sizeof(T);

            std::memcpy(this->m_context.stackPointer, &var, sizeof(T));
        }
    };
}

