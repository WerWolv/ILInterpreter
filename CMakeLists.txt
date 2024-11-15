cmake_minimum_required(VERSION 3.15)
project(CSharpInterpreter)

set(CMAKE_CXX_STANDARD 23)

add_executable(CSharpInterpreter source/main.cpp source/dll.cpp source/method.cpp source/logger.cpp source/native.cpp)
target_include_directories(CSharpInterpreter PRIVATE include)

add_subdirectory(rewrite)