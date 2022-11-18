cmake_minimum_required(VERSION 3.20)

add_subdirectory(./lib/Physics)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

project(quantum_simulator)

set(QC_EXCUTABLE three_bit_adder n_qbit_adder)

foreach(M ${QC_EXCUTABLE})
    add_executable(${M} src/${M}.cc)

    target_compile_options(
        ${M}
        PRIVATE
        -std=c++20
    )

    target_include_directories(
        ${M}
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
    )

    target_link_libraries(
        ${M}
        physics
    )

endforeach()

