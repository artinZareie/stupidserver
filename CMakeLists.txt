cmake_minimum_required(VERSION 3.10)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(stupidserver VERSION 0.0.1)

find_package(Threads REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)

set(SOURCES main.c src/helpers.c)
set(HEADERS src/helpers.h)

add_executable(stupidserver ${SOURCES} ${HEADERS})

target_link_libraries(stupidserver PRIVATE Threads::Threads)
