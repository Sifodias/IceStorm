cmake_minimum_required(VERSION 3.0.0)
project(IceStorm)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../src/")
set(PROJECT_BINARY_DIR "src/")
include_directories("libs/imgui-1.82")

file(GLOB_RECURSE CPPS "src/*.cpp")
file(GLOB_RECURSE LIBS "libs/imgui-1.82/*.cpp")
# file(GLOB_RECURSE LIBS2 "libs/imgui-1.82/backends/*.cpp")
add_executable(IceStorm ${CPPS} ${LIBS})

target_link_libraries(IceStorm -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf)
