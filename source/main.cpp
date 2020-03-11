#include "dll.hpp"
#include "tables.hpp"
#include "method.hpp"

static void loadExecutable(std::string path) {
    static ili::Context context;

    context.dll = new ili::DLL(path);
    context.dll->validate();

    context.stack = new u8[context.dll->getStackSize()];
    context.typeStack = new Type[context.dll->getStackSize()];

    context.stackPointer = context.stack;
    context.framePointer = nullptr;
    context.typeStackPointer = context.typeStack;
    context.typeFramePointer = nullptr;

    // Execute Main
    {
        auto entryPoint = new ili::Method(context, context.dll->getEntryMethodToken());
        entryPoint->execute();
    }

    delete[] context.typeStack;
    delete[] context.stack;
    delete   context.dll;
}

int main() {
    loadExecutable("Test.exe");

    return 0;
}
