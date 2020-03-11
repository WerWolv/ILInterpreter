#pragma once

#include "types.hpp"

#include <stack>
#include <memory>
#include <string>
#include <unordered_map>
#include <functional>
#include <cstring>

namespace ili {

    struct VariableBase {
        Type type;
    };

    template<typename T>
    struct Variable : public VariableBase {
        T value;
    };

    class Method;
    class DLL;


    struct Context {
        DLL *dll = nullptr;

        u8 *stackPointer = nullptr;
        u8 *framePointer = nullptr;
        u8 *stack;

        Type *typeStackPointer = nullptr;
        Type *typeFramePointer = nullptr;
        Type *typeStack;

        std::unordered_map<std::string, std::function<void()>> nativeFunctions;



        Type getTypeOnStack() {
            return *(typeStackPointer - 1);
        }

        template<typename T>
        T pop() {
            typeStackPointer--;
            stackPointer -= sizeof(T);
            T ret;

            std::memset(&ret, 0x00, sizeof(T));
            std::memcpy(&ret, stackPointer, getTypeSize(getTypeOnStack()));

            return ret;
        }

        template<typename T>
        void push(Type type, T val) {

            std::memcpy(stackPointer, &val, getTypeSize(type));

            *typeStackPointer = type;
            typeStackPointer++;
            stackPointer += sizeof(T);
        }
    };

}