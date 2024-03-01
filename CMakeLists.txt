cmake_minimum_required(VERSION 3.20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(
    PkgConfig
    REQUIRED
)

pkg_check_modules(
    RAYLIB
    REQUIRED
    raylib
)

project(
    Boltor
    VERSION 0.1
    DESCRIPTION "Boltor - A fun game engine"
    LANGUAGES C
)

set_property(
    TARGET ${PROJECT}
    PROPERTY C_STANDARD 11
)

add_library(
    ${PROJECT_NAME}
    src/boltor.c
)

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC
    include
    ${RAYLIB_INCLUDE_DIRS}
)

add_executable(
    Game
    src/game.c
)

target_link_libraries(
    Game
    ${RAYLIB_LIBRARIES}
    m
    ${PROJECT_NAME}
)

target_link_directories(
    Game
    PUBLIC
    build
    ${RAYLIB_LIBRARY_DIRS}
)

target_include_directories(
    Game
    PUBLIC
    include
)