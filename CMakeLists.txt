cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(stupidserver VERSION 0.0.1)

set(SOURCES main.c src/helpers.c)
set(HEADERS src/helpers.h)

add_executable(stupidserver ${SOURCES} ${HEADERS})
