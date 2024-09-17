cmake_minimum_required(VERSION 3.21)

add_executable(dump_eeprom)

target_sources(dump_eeprom PRIVATE
    dump_eeprom.c
)
target_include_directories(dump_eeprom PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
target_compile_options(dump_eeprom PRIVATE
    -Wall
)
target_link_libraries(dump_eeprom
    pico_stdlib
    swi_eeprom
    hardware_pio
)
pico_add_extra_outputs(dump_eeprom)
