cmake_minimum_required(VERSION 3.12)

add_executable(raspi_pico_template)

pico_generate_pio_header(raspi_pico_template
    ${CMAKE_CURRENT_LIST_DIR}/eeprom.pio
)

target_sources(raspi_pico_template PRIVATE
    main.c
)
target_include_directories(raspi_pico_template PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
target_compile_options(raspi_pico_template PRIVATE
    -Wall
)
target_link_libraries(raspi_pico_template
    pico_stdlib
    hardware_pio
)

pico_add_extra_outputs(raspi_pico_template)
