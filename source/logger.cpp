#include <cstdio>
#include <cstdarg>
#include "logger.hpp"

namespace ili {

    void Logger::error(const char *format, ...) {
        va_list ap;
        va_start(ap, format);
        printf("\033%s[%s]\033[0m ", "[0;31m", "ERROR");
        vprintf(format, ap);
        printf("\n");
        va_end(ap);
    }

    void Logger::info(const char *format, ...) {
        va_list ap;
        va_start(ap, format);
        printf("\033%s[%s]\033[0m  ", "[0;34m", "INFO");
        vprintf(format, ap);
        printf("\n");
        va_end(ap);
    }

    void Logger::debug(const char *format, ...) {
        if (!Logger::DebugLogging)
            return;

        va_list ap;
        va_start(ap, format);
        printf("\033%s[%s]\033[0m ", "[0;32m", "DEBUG");
        vprintf(format, ap);
        printf("\n");
        va_end(ap);
    }

}