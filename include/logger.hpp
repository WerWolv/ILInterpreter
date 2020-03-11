#pragma once

namespace ili {

    class Logger {
    public:
        static constexpr bool DebugLogging = true;

        static void error(const char *format, ...);
        static void info(const char *format, ...);
        static void debug(const char *format, ...);
    };

}