#
# Single-Wire Interface EEPROM driver for Raspberry Pi Pico
#
cmake_minimum_required(VERSION 3.21)

# load pico-sdk
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(singlewire_eeprom
    VERSION 0.1.0
    DESCRIPTION "Single-Wire Interface (SWI) EEPROM driver library"
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

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wno-maybe-uninitialized)
endif()

option(SWI_EEPROM_BUILD_SAMPLES "Build stand-alone samples" OFF)

add_subdirectory(src)

if(SWI_EEPROM_BUILD_SAMPLES)
    add_subdirectory(samples)
endif()
