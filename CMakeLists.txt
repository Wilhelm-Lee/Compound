cmake_minimum_required (VERSION 3.5)

project (Compound)

add_compile_options(-g -std=c99 -Wall -Wextra -Wformat)

# add_executable(CompoundTest test.c
#                Var/src/var.c
#                Status/src/status.c
#                Stack/src/stack.c
#                Array/src/array.c
#                Utils/src/utils.c
#                catlog.c
#                name.c)

add_executable(CompoundTest test.c
               Var/src/var.c
               Array/src/array.c
               Status/src/status.c)
