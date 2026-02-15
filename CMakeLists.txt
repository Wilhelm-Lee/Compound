cmake_minimum_required(VERSION 3.20)

project(Compound C)

set(CMAKE_C_STANDARD 99)

set(SOURCES
        src/arrays.c
        src/memory.c
        src/string.c
)

include(FetchContent)

FetchContent_Declare(
        safestringlib
        GIT_REPOSITORY https://github.com/intel/safestringlib.git
        GIT_TAG master  # or specify a specific commit hash/tag for stability
)

FetchContent_MakeAvailable(safestringlib)

add_library(compound SHARED
        ${SOURCES}
)

# Rename the executable to avoid conflict with the library 'compound'
# On MacOS/Windows 'Compound' == 'compound', which breaks the build.
add_executable(CompoundTest
        ${SOURCES}
        test/test.c
)

target_link_libraries(compound PRIVATE safestring_shared)
target_link_libraries(CompoundTest PRIVATE safestring_shared)
