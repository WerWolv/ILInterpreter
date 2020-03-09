#include "dll.hpp"
#include "tables.hpp"

int main() {
    csharp::DLL dll("Test2.exe");
    dll.validate();
    csharp::table_method_def_t* entryPointMethod = dll.getMethodByToken(dll.getEntryMethodToken());
    printf("Entry point method: %s\n", dll.getString(entryPointMethod->nameIndex));
    dll.execute(entryPointMethod);
    return 0;
}
