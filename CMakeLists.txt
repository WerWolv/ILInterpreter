cmake_minimum_required(VERSION 3.16)
project(CSharpInterpreter)

include_directories(CSharpInterpreter include)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

add_executable(CSharpInterpreter source/main.cpp source/dll.cpp source/method.cpp source/logger.cpp source/native.cpp)