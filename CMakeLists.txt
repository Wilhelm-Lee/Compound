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

#include(FetchContent)
#FetchContent_Declare(
#  Compound.Hashmap
#  GIT_REPOSITORY https://github.com/Wilhelm-Lee/Compound.Hashmap.git
#  GIT_TAG 6b77fe06c5ced6bb0f8eb70a281e7ee582a75b19
#)
#FetchContent_MakeAvailable(Compound.Hashmap)


set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(
  SOURCES
  src/arrays.c
  src/init.c
  src/memory_stack.c
  src/string.c
)


#add_library(
#  hashmap
#  SHARED
#  ${hashmap_SOURCE_DIR}/bucket.c
#  ${hashmap_SOURCE_DIR}/hashmap.c
#)
# Rename the executable to avoid conflict with the library 'compound' On
# MacOS/Windows 'Compound' == 'compound', which breaks the build.
add_library(CommonCompilationFlags INTERFACE)
add_library(Objects OBJECT ${SOURCES})
add_library(compound SHARED $<TARGET_OBJECTS:Objects>)

target_compile_options(
  CommonCompilationFlags
  INTERFACE
  -std=c99
  -Wall
  -Wextra
  -Werror
  -g3
  -O0
  -fPIC
  # -fno-omit-frame-pointer  # __builtin_frame_address(1) for feature Backtracing.
  -D__COMPOUND_FEATURECLASS_COMMANDLINE__
  -D__COMPOUND_FEATURE_RECYCLER__
  -D__COMPOUND_FEATURE_BOOLEAN__
)

add_executable(CompoundTest test/test.c)
# add_executable(CompoundDemo test/demo.c)

# CORRECTED: Objects only needs the compilation flags, not the 'compound' library.
target_link_libraries(Objects PRIVATE CommonCompilationFlags)

# Executables link against both the final shared library AND the compilation flags.
target_link_libraries(CompoundTest PRIVATE compound CommonCompilationFlags)
# target_link_libraries(CompoundDemo PRIVATE compound CommonCompilationFlags)

#target_include_directories(hashmap PUBLIC ${hashmap_SOURCE_DIR})
#target_link_libraries(CompoundTest PUBLIC hashmap)
#target_link_libraries(compound PUBLIC hashmap)
