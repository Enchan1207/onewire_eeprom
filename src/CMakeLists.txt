cmake_minimum_required(VERSION 3.12)

add_executable(raspi_pico_template)
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
)

pico_add_extra_outputs(raspi_pico_template)
