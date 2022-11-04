cmake_minimum_required(VERSION 3.20)


add_subdirectory(./lib/Physics)


project(quantum_simulator)

file(GLOB MAIN_SOURCES ./src/main.cc)

add_executable(quantum_simulator ${MAIN_SOURCES})

target_link_libraries(
    quantum_simulator
    physics
)

