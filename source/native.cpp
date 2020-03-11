#include "native.hpp"

#include "context.hpp"
#include "dll.hpp"

namespace ili {

    void NativeMethods::loadNXLibrary(Context &ctx) {
        ctx.nativeFunctions.insert({ "NX::NX.Console.WriteLine", [&ctx]{ printf("%s\n", ctx.dll->decodeUserString(ctx.pop<u32>()).c_str()); } });
    }

}