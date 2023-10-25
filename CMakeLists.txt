cmake_minimum_required(VERSION 3.16.3)

project(chip8)

# Set flags
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)

# Inclui arquivos e dependencias
find_package(SDL2 REQUIRED)
include_directories(${SDL_INCLUDE_DIRS})

include_directories(src)
include_directories(include)
file(GLOB SRC "src/*.cpp")
file(GLOB INC "include/*.hpp")

# Compila
add_executable(chip8 ${SRC} ${INC})
target_compile_options(chip8 PRIVATE -std=c++11)
target_link_libraries(chip8 ${SDL2_LIBRARIES})