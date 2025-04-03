cmake_minimum_required(VERSION 3.20)
project(cmd_clip LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_RELEASE "/O2")
set(ZIP_OUTPUT_PATH "${CMAKE_BINARY_DIR}/cmd_clip.zip")

foreach(cmd fclear fcopy fcut flist fpaste)
	add_executable(${cmd} src/${cmd}.cpp)
endforeach()
add_custom_target(zip_binaries ALL COMMAND powershell -Command "Compress-Archive -Path '${CMAKE_BINARY_DIR}/fclear.exe', '${CMAKE_BINARY_DIR}/fcopy.exe', '${CMAKE_BINARY_DIR}/fcut.exe', '${CMAKE_BINARY_DIR}/flist.exe', '${CMAKE_BINARY_DIR}/fpaste.exe' -DestinationPath '${ZIP_OUTPUT_PATH}'" DEPENDS ${cmd})
