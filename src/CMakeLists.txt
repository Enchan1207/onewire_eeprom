cmake_minimum_required(VERSION 3.12)

add_library(swi_eeprom)

pico_generate_pio_header(swi_eeprom
    ${CMAKE_CURRENT_LIST_DIR}/driver.pio
)

target_sources(swi_eeprom PRIVATE
    driver.c
    util.c
)
target_include_directories(swi_eeprom PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)
target_compile_options(swi_eeprom PRIVATE
    -Wall
)
target_link_libraries(swi_eeprom PUBLIC
    pico_stdlib
    hardware_pio
)
