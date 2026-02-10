cmake_minimum_required(VERSION 4.1)
project(Compound C)

set(CMAKE_C_STANDARD 99)

set(SOURCES
        src/arrays.c
        src/memory.c
        src/string.c
)

add_library(compound SHARED
        ${SOURCES}
)

add_executable(Compound
        ${SOURCES}
        test/test.c
)
