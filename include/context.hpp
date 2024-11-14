#pragma once

#include "types.hpp"

#include <memory>
#include <string>
#include <unordered_map>
#include <list>
#include <functional>
#include <cstring>
#include "logger.hpp"

namespace ili {

    struct VariableBase {
        Type type;
    };

    template<typename T>
    struct Variable : public VariableBase {
        T value;
    };

    struct HeapReference {
        u8 *heapPointer;
        size_t size;
    };

    class Method;
    class DLL;


    struct Context {
        DLL *dll = nullptr;

        u8 *heap = nullptr;
        std::list<HeapReference> heapReferences;

        u8 *stackPointer = nullptr;
        u8 *framePointer = nullptr;
        u8 *stack;

        Type *typeStackPointer = nullptr;
        Type *typeFramePointer = nullptr;
        Type *typeStack;

        std::unordered_map<std::string, std::function<void()>> nativeFunctions;



        Type getTypeOnStack(u16 pos = 0) {
            return *(typeStackPointer - 1 - pos);
        }

        template<typename T>
        T pop() {
            T ret = {};

            if (stackPointer <= stack) {
                Logger::error("Popped %d bytes from the stack but the stack is empty!", sizeof(T));
                exit(1);
            }

            size_t sizeToPop = getTypeSize(getTypeOnStack());

            if (sizeToPop > sizeof(T)) {
                Logger::error("Popped %d bytes into %d byte return value!", sizeToPop, sizeof(T));
                exit(1);
            }

            typeStackPointer--;
            stackPointer -= sizeof(T);

            if (stackPointer < stack) {
                Logger::error("Popped %d which was more than the stack held!", sizeof(T));
                exit(1);
            }

            std::memset(&ret, 0x00, sizeof(T));
            std::memcpy(&ret, stackPointer, sizeToPop);

            Logger::debug("Popped %d bytes from stack: %016llx", sizeof(T), ret);

            return ret;
        }

        template<typename T>
        void push(Type type, T val) {

            std::memcpy(stackPointer, &val, getTypeSize(type));
            *typeStackPointer = type;

            typeStackPointer++;
            stackPointer += sizeof(T);

            Logger::debug("Pushed %d bytes onto stack: %016llx", sizeof(T), val);
        }

        u32 getUsedStackSize() {
            return this->stackPointer - this->stack;
        }
    };

}