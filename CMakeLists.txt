cmake_minimum_required(VERSION 3.0.0)
project(IceStorm)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../src/")
set(PROJECT_BINARY_DIR "src/")

file(GLOB_RECURSE CPPS "src/*.cpp")
add_executable(IceStorm ${CPPS})
target_link_libraries(IceStorm -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf)
