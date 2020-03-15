#include <logger.hpp>
#include "dll.hpp"
#include "native.hpp"
#include "method.hpp"

static void loadExecutable(std::string path) {
    static ili::Context context;

    context.dll = new ili::DLL(path);
    context.dll->validate();

    context.heap = new u8[0x0010'0000];

    context.stack = new u8[context.dll->getStackSize()];
    context.typeStack = new Type[context.dll->getStackSize()];

    context.stackPointer = context.stack;
    context.framePointer = nullptr;
    context.typeStackPointer = context.typeStack;
    context.typeFramePointer = nullptr;

    ili::NativeMethods::loadNXLibrary(context);

    // Execute Main
    {
        auto entryPoint = std::make_unique<ili::Method>(context, context.dll->getEntryMethodToken());
        ili::Variable<s32>* ret = static_cast<ili::Variable<s32>*>(entryPoint->run());
        ili::Logger::info("Program finished with exit code %d", 0);
    }

    delete[] context.typeStack;
    delete[] context.stack;
    delete[] context.heap;
    delete   context.dll;
}

int main() {
    loadExecutable("Test2.exe");

    return 0;
}
