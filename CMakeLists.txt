cmake_minimum_required(VERSION 3.20)
project(cmd_clip LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "/O2")

foreach(cmd fclear fcopy fcut flist fpaste)
	add_executable(${cmd} src/${cmd}.cpp)
endforeach()
