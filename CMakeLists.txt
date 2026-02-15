cmake_minimum_required(VERSION 3.20)

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

# Rename the executable to avoid conflict with the library 'compound'
# On MacOS/Windows 'Compound' == 'compound', which breaks the build.
add_executable(CompoundTest
        ${SOURCES}
        test/test.c
)
