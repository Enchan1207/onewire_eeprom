cmake_minimum_required(VERSION 3.21)

macro(add_sample program_name)
    add_executable(${program_name})

    target_sources(${program_name} PRIVATE
        ${program_name}.c
    )
    target_include_directories(${program_name} PRIVATE
        ${PROJECT_SOURCE_DIR}/include
    )
    target_compile_options(${program_name} PRIVATE
        -Wall
    )
    target_link_libraries(${program_name}
        pico_stdlib
        swi_eeprom
        hardware_pio
    )
    pico_add_extra_outputs(${program_name})
endmacro()

add_sample(dump_eeprom)
add_sample(write_eeprom)
