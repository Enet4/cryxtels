cmake_minimum_required(VERSION 3.10)

project(cryxtels VERSION 1.1.0)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_SOURCE_DIR}")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

# SDL3
find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3)

## SDL3_mixer
find_package(SDL3_mixer REQUIRED)

# project includes
include_directories(include)

# add the executable
add_executable(cryxtels
    source/cryxtels.cpp source/fast3d.cpp source/global.cpp source/input.cpp
    source/text3d.cpp source/draw.cpp source/transition.cpp source/intro.cpp
    source/conf.cpp source/dsp.cpp)
target_link_libraries(cryxtels PRIVATE
    SDL3::SDL3 $<IF:$<TARGET_EXISTS:SDL3_mixer::SDL3_mixer>,SDL3_mixer::SDL3_mixer,SDL3_mixer>)
