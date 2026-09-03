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

set(USER_DIRECTORY "${PROJECT_SOURCE_DIR}/user")
set(USER_HEADER "${USER_DIRECTORY}/header.h")

# Setup environments for source code generation.
file(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/user")
file(TOUCH "${USER_HEADER}")

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
  # src/heap.c
  src/access.c
  src/arrays.c
  src/arrays_lit.c
  src/body.c
  src/buffer.c
  src/class.c
  src/constructor.c
  src/destructor.c
  src/field.c
  src/function.c
  src/init.c
  src/match.c
  src/memory_stack.c
  src/method.c
  src/parameter.c
  src/preprocessor.c
  src/regex.c
  src/signature.c
  src/stream.c
  src/string.c
)


find_package(PkgConfig REQUIRED)
pkg_check_modules(PCRE2 REQUIRED libpcre2-8)


#add_library(
#  hashmap
#  SHARED
#  ${hashmap_SOURCE_DIR}/bucket.c
#  ${hashmap_SOURCE_DIR}/hashmap.c
#)
# Rename the executable to avoid conflict with the library 'compound' On
# MacOS/Windows 'Compound' == 'compound', which breaks the build.
add_library(CommonCompilationFlags INTERFACE)
add_library(Fundation OBJECT ${SOURCES})
add_library(compound SHARED $<TARGET_OBJECTS:Fundation>)

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
  # -D__COMPOUND_FEATURECLASS_COMMANDLINE__
  # -D__COMPOUND_FEATURE_ARGUMENT__
  -D__COMPOUND_FEATURE_RECYCLER__
  -D__COMPOUND_FEATURE_BOOLEAN__
)

add_executable(CompoundTest test/test.c)
# add_executable(CompoundDemo test/demo.c)

target_include_directories(Fundation PUBLIC ${PCRE2_INCLUDE_DIRS}})
target_include_directories(compound PUBLIC ${PCRE2_INCLUDE_DIRS}})

target_link_libraries(Fundation PRIVATE CommonCompilationFlags)
target_link_libraries(Fundation PUBLIC ${PCRE2_LIBRARIES})
target_link_libraries(compound PUBLIC ${PCRE2_LIBRARIES})

# Executables link against both the final shared library AND the compilation flags.
target_link_libraries(CompoundTest PRIVATE compound CommonCompilationFlags)
# target_link_libraries(CompoundDemo PRIVATE compound CommonCompilationFlags)

#target_include_directories(hashmap PUBLIC ${hashmap_SOURCE_DIR})
#target_link_libraries(CompoundTest PUBLIC hashmap)
#target_link_libraries(compound PUBLIC hashmap)
