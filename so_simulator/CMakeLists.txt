cmake_minimum_required(VERSION 4.0)
project(so_simulator)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(simulator
    main.cpp
    ${SOURCES}
)

target_include_directories(simulator PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
