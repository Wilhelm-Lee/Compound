cmake_minimum_required (VERSION 3.5)

project (Compound)

set(CMAKE_C_COMPILER  gcc)

set(WALL  -Waddress
          -Warray-compare
          -Warray-parameter=2
          -Wbool-compare
          -Wbool-operation
          -Wchar-subscripts
          -Wcomment
          -Wdangling-else
          -Wdangling-pointer=2
          -Wenum-compare
          -Wenum-int-mismatch
          -Wformat=1
          -Wformat-contains-nul
          -Wformat-diag
          -Wformat-extra-args
          -Wformat-overflow=1
          -Wformat-truncation=1
          -Wformat-zero-length
          -Wframe-address
          -Wimplicit
          -Wimplicit-function-declaration
          -Wimplicit-int
          -Winfinite-recursion
          -Wint-in-bool-context
          -Wlogical-not-parentheses
          -Wmain
          -Wmaybe-uninitialized
          -Wmemset-elt-size
          -Wmemset-transposed-args
          -Wmisleading-indentation
          -Wmismatched-dealloc
          -Wmissing-attributes
          -Wmissing-braces
          -Wmultistatement-macros
          -Wnonnull
          -Wnonnull-compare
          -Wopenmp-simd
          -Wpacked-not-aligned
          -Wparentheses
          -Wpointer-sign
          -Wrestrict
          -Wreturn-type
          -Wsequence-point
          -Wsizeof-array-div
          -Wsizeof-pointer-div
          -Wsizeof-pointer-memaccess
          -Wstrict-aliasing
          -Wstrict-overflow=1
          -Wswitch
          -Wtautological-compare
          -Wtrigraphs
          -Wuninitialized
          -Wunknown-pragmas
          -Wunused
          -Wunused-but-set-variable
          -Wunused-const-variable=1
          -Wunused-function
          -Wunused-label
          -Wunused-local-typedefs
          -Wunused-value
          -Wunused-variable
          -Wuse-after-free=2
          -Wvla-parameter
          -Wvolatile-register-var
          -Wzero-length-bounds
)

add_compile_options(-g -std=c99 ${WALL} -Wextra -D__DEBUG__)

set(SHARED_SOURCE
    MemMan/src/memman.c
    Status/src/status.c
    Utils/src/utils.c
    Var/src/var.c
    catlog.c)

set(LIBCOMPOUND_SOURCE ${SHARED_SOURCE})

add_library(compound SHARED ${LIBCOMPOUND_SOURCE})

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
