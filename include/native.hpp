#pragma once

#include <string>
#include <functional>

namespace ili {

    class Context;

    class NativeMethods {
    public:
        static void loadMSCORLIBLibrary(Context &ctx);
        static void loadNXLibrary(Context &ctx);

        static void registerMethod(Context &ctx, std::string methodName, std::function<void()> method);
        static void callMethod(Context &ctx, std::string methodName);
    };

}

