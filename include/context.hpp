#pragma once

#include <stack>
#include <memory>
#include <string>

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
    };

}