#include "native.hpp"

#include "context.hpp"
#include "dll.hpp"



namespace ili {

    void NativeMethods::registerMethod(Context &ctx, std::string methodName, std::function<void()> method) {
        ctx.nativeFunctions.insert({ methodName, method });
    }

    void NativeMethods::callMethod(Context &ctx, std::string methodName) {
        ctx.nativeFunctions[methodName]();
    }


    void NativeMethods::loadMSCORLIBLibrary(Context &ctx) {
        registerMethod(ctx, "[mscorlib]System.Object::.ctor", []{ /* ... */ } );
        registerMethod(ctx, "[mscorlib]System.Console::WriteLine", [&ctx]{ callMethod(ctx, "[NX]NX.Console::WriteLine"); } );
    }

    void NativeMethods::loadNXLibrary(Context &ctx) {
        registerMethod(ctx, "[NX]NX.Console::WriteLine", [&ctx]{ printf("%s\n", ctx.dll->decodeUserString(ctx.pop<u32>()).c_str()); } );
    }

}