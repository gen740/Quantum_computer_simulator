cmake_minimum_required(VERSION 3.20)



add_subdirectory(./lib/Physics)


set(CMAKE_EXPORT_COMPILE_COMMANDS)
project(quantum_simulator)

file(GLOB MAIN_SOURCES ./src/main.cc)

add_executable(quantum_simulator ${MAIN_SOURCES})

target_compile_options(
    quantum_simulator
    PRIVATE
    -std=c++20
)

target_link_libraries(
    quantum_simulator
    physics
)

target_include_directories(
    quantum_simulator
    PUBLIC
    ${CMAKE_CURREST_LIST_DIR}/include
    )

