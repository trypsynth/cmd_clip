cmake_minimum_required(VERSION 3.20)
project(cmd_clip LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>") # Allow /MT to actually take effect.
set(CMAKE_CXX_FLAGS_RELEASE "/O2 /MT")

add_executable(fcopy src/fcopy.cpp)
add_executable(fpaste src/fpaste.cpp)
