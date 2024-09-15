cmake_minimum_required(VERSION 3.12)

add_library(eeprom)

pico_generate_pio_header(eeprom
    ${CMAKE_CURRENT_LIST_DIR}/eeprom.pio
)

target_sources(eeprom PRIVATE
    driver.c
    util.c
)
target_include_directories(eeprom PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
target_compile_options(eeprom PRIVATE
    -Wall
)
target_link_libraries(eeprom
    pico_stdlib
    hardware_pio
)
