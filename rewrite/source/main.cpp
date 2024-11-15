#include <ili/executable_file.hpp>
#include <ili/runtime.hpp>

int main() {
    ili::Runtime runtime;

    runtime.run(ili::Executable("test/example/bin/Debug/net8.0/win-x64/example.dll"));

    return EXIT_SUCCESS;
}
