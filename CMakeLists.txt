#
# CMake project template for Raspberry Pi Pico
#
# reference: https://github.com/raspberrypi/pico-examples/blob/master/CMakeLists.txt
#
cmake_minimum_required(VERSION 3.12)

## load pico-sdk
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(pico_project_example
    VERSION 0.1.0
    DESCRIPTION "CMake project template for Raspberry Pi Pico"
    LANGUAGES C CXX ASM
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

pico_sdk_init()

add_compile_options(
    -Wno-format
    -Wno-unused-function
)
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wno-maybe-uninitialized)
endif()

add_subdirectory(src)
