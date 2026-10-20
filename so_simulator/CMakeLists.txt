cmake_minimum_required(VERSION 3.21)
project(so_simulator LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compilar estaticamente as bibliotecas que respeitam essa opção.
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
set(PLATFORM Desktop CACHE STRING "" FORCE)

include(FetchContent)

# Pin mutually compatible versions so a clean checkout can build the UI.
FetchContent_Declare(raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(raylib)

FetchContent_Declare(dearimgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.92.7
    GIT_SHALLOW TRUE
)
FetchContent_GetProperties(dearimgui)
if(NOT dearimgui_POPULATED)
    FetchContent_MakeAvailable(dearimgui)
endif()

FetchContent_Declare(rlimgui
    GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
    GIT_TAG Raylib_5_5
    GIT_SHALLOW TRUE
    GIT_SUBMODULES_RECURSE TRUE
)
FetchContent_GetProperties(rlimgui)
if(NOT rlimgui_POPULATED)
    FetchContent_MakeAvailable(rlimgui)
endif()

FetchContent_Declare(tinyfiledialogs
    GIT_REPOSITORY https://github.com/native-toolkit/tinyfiledialogs.git
    GIT_TAG cc6b593c029110af8045826ce691f540c85e850c
    GIT_SHALLOW TRUE
)
FetchContent_GetProperties(tinyfiledialogs)
if(NOT tinyfiledialogs_POPULATED)
    FetchContent_MakeAvailable(tinyfiledialogs)
endif()

add_library(imgui STATIC
    "${dearimgui_SOURCE_DIR}/imgui.cpp"
    "${dearimgui_SOURCE_DIR}/imgui_draw.cpp"
    "${dearimgui_SOURCE_DIR}/imgui_tables.cpp"
    "${dearimgui_SOURCE_DIR}/imgui_widgets.cpp"
    "${rlimgui_SOURCE_DIR}/rlImGui.cpp"
)
target_include_directories(imgui PUBLIC "${dearimgui_SOURCE_DIR}" "${rlimgui_SOURCE_DIR}")
target_link_libraries(imgui PUBLIC raylib)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_executable(simulator
    main.cpp
    ${SOURCES}
    "${tinyfiledialogs_SOURCE_DIR}/tinyfiledialogs.c"
)
target_include_directories(simulator PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${tinyfiledialogs_SOURCE_DIR}"
)
target_compile_definitions(simulator PRIVATE
    DEFAULT_CONFIG_PATH="${CMAKE_CURRENT_SOURCE_DIR}/config-files/teste1.txt"
)
target_link_libraries(simulator PRIVATE raylib imgui)

if(WIN32)
    target_link_libraries(simulator PRIVATE comdlg32)
endif()
