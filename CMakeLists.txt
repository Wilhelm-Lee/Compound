cmake_minimum_required(VERSION 3.20)
project(Compound C)

set(CMAKE_BINARY_DIR ${PROJECT_SOURCE_DIR}/out)

# Custom directory for storing generated object files by CMake.
set(CMAKE_BUILD_DIR ${CMAKE_BINARY_DIR}/.build)

# Executables & DLLs (Windows)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Shared Objects (Linux)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Static Libraries / Import Libraries
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

set(CMAKE_CACHEFILE_DIR ${CMAKE_BUILD_DIR})

set(USER_DIRECTORY "${PROJECT_SOURCE_DIR}/usr")
set(USER_HEADER "${USER_DIRECTORY}/header.h")

set(DEP_DIRECTORY "${PROJECT_SOURCE_DIR}/dep")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(
  SOURCES
  src/access.c
  src/allocator.c
  src/arrays.c
  src/arrays_lit.c
  src/body.c
  src/buffer.c
  src/class.c
  src/constructor.c
  src/destructor.c
  src/field.c
  src/function.c
  src/heap.c
  src/init.c
  src/match.c
  src/memory_stack.c
  src/memusage.c
  src/method.c
  src/origin.c
  src/parameter.c
  src/preprocessor.c
  src/recollector.c
  src/regex.c
  src/signature.c
  src/stream.c
  src/string.c
)


# Setup environments for source code generation.
file(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/usr")
file(TOUCH "${USER_HEADER}")


file(MAKE_DIRECTORY "${DEP_DIRECTORY}")
find_package(Git QUIET)
if(NOT GIT_FOUND)
  message(FATAL_ERROR "Cannot find the git executable.  Make sure you have read README.md for necessary dependencies to build this project.")
endif()

execute_process(
    COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
    WORKING_DIRECTORY "${DEP_DIRECTORY}"
    RESULT_VARIABLE git_result
)

if(NOT ${git_result} EQUAL 0)
  message(FATAL_ERROR "Failed to execute: ${GIT_EXECUTABLE} submodule update --init --recursive; exit code: ${git_result}")
endif()


# Dependencies.  (git submodule-ed)
add_subdirectory(dep/pcre2)


# Rename the executable to avoid conflict with the library 'compound' On
# MacOS/Windows 'Compound' == 'compound', which breaks the build.
add_library(CommonDependencies INTERFACE)
add_library(CommonCompilationFlags INTERFACE)
add_library(Foundation OBJECT ${SOURCES})
add_library(compound SHARED $<TARGET_OBJECTS:Foundation>)

target_compile_options(
  CommonCompilationFlags
  INTERFACE
  -std=iso9899:1999
  -Wall
  -Wextra
  -Werror
  -Wpedantic
  -g3
  -O0
  -fPIC
  # -fno-omit-frame-pointer  # __builtin_frame_address(1) for feature Backtracing.
  # -D__COMPOUND_FEATURE_HEAP__
  # -D__COMPOUND_FEATURE_ARGUMENT__
  -D__COMPOUND_FEATURE_RECYCLER__
  -D__COMPOUND_FEATURE_BOOLEAN__
  # -D__COMPOUND_FEATURE_RECOLLECTOR__
)

add_executable(CompoundTest test/test.c)

target_link_libraries(CommonDependencies INTERFACE pcre2-8)
target_link_libraries(
  Foundation
    PRIVATE CommonCompilationFlags
    PUBLIC CommonDependencies
)
target_link_libraries(compound PUBLIC CommonDependencies)

target_link_libraries(CompoundTest PRIVATE compound CommonCompilationFlags)
