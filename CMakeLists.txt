cmake_minimum_required (VERSION 3.5)

project (Compound)

set(SHARED_SOURCE
    Status/src/status.c
    Utils/src/utils.c
    Var/src/var.c
    catlog.c)

set(LIBCOMPOUND_SOURCE ${SHARED_SOURCE})

add_library(compound SHARED ${LIBCOMPOUND_SOURCE})

add_compile_options(-g -std=c99 -Wall -Wextra -Wformat)
LINK_LIBRARIES(m)

# add_executable(CompoundTest test.c
#                Var/src/var.c
#                Status/src/status.c
#                Stack/src/stack.c
#                Array/src/array.c
#                Utils/src/utils.c
#                catlog.c
#                name.c)

add_executable(CompoundTest
               test.c
               MemMan/src/memman.c
               Status/src/status.c
               Utils/src/utils.c
               catlog.c)
