# COMPOUND

  The Compound library is a macro-driven, C99-compliant framework designed to
  bring high-level object-oriented patterns and automated memory management to
  the C programming language. It provides a robust foundation for building
  complex systems by abstracting common tasks such as generic container
  management, string manipulation, and programmatically generated C code.

  > To know more about generative source code licensing notice, see [Important Notice Regarding Generated Code](#important-notice-regarding-generated-code)

  Compound addresses the fragility of manual memory management in C through a
  built-in memory pool and recycler system, while maintaining performance and
  portability across multiple compilers and platforms.

## Building

  [![Codacy Badge](https://app.codacy.com/project/badge/Grade/f653f688f7034e1bb842a87849809ac1)](https://app.codacy.com/gh/Wilhelm-Lee/Compound/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)
  <!-- > [Codacy Dashboard](https://app.codacy.com/gh/Wilhelm-Lee/Compound/dashboard) -->

  Compound supports multiple platforms, and is built using CMake.

  This section delivers **minimum steps** for building Compound on your machine.

   * [Microsoft Windows](#microsoft-windows)
   * [GNU/Linux & Apple MacOS](#gnulinux--apple-macos)
   * [Other Platforms](#other-platforms)

   The output is managed.  See [Filesystem](#filesystem).

### Microsoft Windows

  Please make sure to have sufficient prerequisites installed on your
  Windows machine before starting the building process.

  It is recommended to have the following software downloaded, and, installed
  on your computer, every one of them:

  > click the item to go to the download page

   - [Git](https://git-scm.com/install/windows)
   - [Microsoft Visual Studio](https://visualstudio.microsoft.com/downloads)
   - [CMake](https://cmake.org/download)
   - [Ninja](https://github.com/ninja-build/ninja/releases) / MSBuild (comes with Visual Studio)

#### PowerShell / CMD

  If you have not downloaded the project from GitHub.com <!-- or CodeBurge.com-->,
  use the following command to do so:
  ```pwsh
    git clone https://github.com/Wilhelm-Lee/Compound.git
    cd Compound
  ```

  Next, build the project using CMake:
  ```pwsh
    cmake .
    make
  ```

  > Please be noticed that due to the uncertainty of the environment for
  > development on Windows, the result of building processes may vary and may
  > require installing extra dependencies manually.

### GNU/Linux & Apple MacOS

  Since you are using Unix-like systems, and you are trying to build a highly
  technical library on your own, it is considered that you have already acquired
  enough knowledge to operate without holding hands.

  So here it is.
  ```bash
    cmake . && make
  ```

## Other Platforms

  If your platform wasn't listed, it's probably untested for the time being.

  However, if your Operating System has more people talking about to add a
  support for it, the develop team is here to consider the suggestion.

## Licensing and Compilation Acknowledgment

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Library General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU Library General Public
  License along with this library; if not, see
  <https://www.gnu.org/licenses/>.

### Important Notice Regarding Generated Code

  This library is designed to be fully usable as a standard, plain library.
  However, it also features an optional self-generating architecture.

  If you choose to enable this feature, the library will generate additional
  source code that must be compiled and executed for those specific extended
  capabilities to function.

  By opting to configure, compile, or execute the generated outcome of this
  library, you explicitly acknowledge and agree to the following:

  *   **Optional Code Generation:** You understand that compiling and executing
      the library's own programmatic output is entirely optional and only
      required if you choose to utilize the self-generating features.
  *   **Derivatives and Linking:** Any resulting binaries or modifications that
      dynamically or statically link to this library-including the optionally
      generated source code-remain subject to the terms of the LGPL 2.0.

## Filesystem

  This section lists the content changes in directories after
  both the **building** and the <u>source code generation</u>.

  * The building changes are highlighted using **bold**.
  * The source code generation changes are highlighted using <u>underlines</u>.

  Before **building** + <u>source code generation</u>:
  * inc
    * \*.h
  * src
    * \*.c
  * test
    * test.c

  After **building** + <u>source code generation</u>:
  * inc
  * src
  * test
    * test.c
  * **out**
    * **bin**
      * **CompoundTest/\~.exe**
    * **lib**
      * **libcompound.so/\~.dylib/\~.dll**
  * **CMakeFiles**
    * **compound.dir**
    * **CompoundTest.dir**
      * **test**
        * **test.c.o**
        * **test.c.o.d**
    * **Fundation.dir**
      * **src**
        * **\*.c.o**
        * **\*.c.o.d**
  * <u>user</u>
    * <u>header.h</u>

## Language Compatibility

  Compound is written compliantly to C99 standard.

  But instead of relying on non-standard compiler extensions, recently obsoleted
  features or platform-specific APIs, Compound achieves wide compatibility
  through strict adherence to standard C paradigms:

### Compound adopts
  *   Strict compliance with the C99 standard; successfully compiled with
      `-Wpedantic` and `-std=iso9899:1999`.
  *   Fixed-width integer definitions exclusively via `<stdint.h>` for unified
      memory layouts.
  *   Standard variadic macros (`__VA_ARGS__`) to drive generic data structures
      and Metaprogramming.
  <!-- *   Predefined compiler macros (`__FILE__`, `__func__`, `__LINE__`) for
      location tracking and error tracing. -->
  *   A wrapped `Main` macro entry point to standardize environment and argument
      parsing across platforms.
  *   Optional, isolated boolean type definitions (`unsigned char`) to avoid
      `<stdbool.h>` namespace collisions.

### Compound forbids
  *   Variable Length Arrays (VLA).
  *   The `#pragma` directive.
  *   The `goto` statement, explicitly undefining it to prevent control flow
      intrusion.
  *   Proprietary compiler extensions for core structural logic.

