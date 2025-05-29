<!--
 * This file is part of Compound library.
 * Copyright (C) 2024-TODAY  William Lee
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * 
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, see
 * <https://www.gnu.org/licenses/>.
 -->

# Compound

Compound is a library that follows C99 standard with OOP supports and is built
with [Tiny C Compiler](https://bellard.org/tcc/), providing
various utilities and data structures with corresponding
manipulations available.  

Enjoy.  :)

<!-- ## Features

Compound offers many appoarches for building robust softwares with the great
compatibility towards early-aged standards.

In general, there are some key features that worth your noticing:

 - **Object Oriented Programming**
 
   Compound supports OOP by providing a neatly structured simulation with the
   great help of macros.
   
   Here is a demonstration of how they are used:
   ```C
   Status Main(void) {
     class (PUBLIC, DYNAMIC, Animal, {
       field (PRIVATE, DYNAMIC, int, age, "");
       field (PRIVATE, DYNAMIC, double, weight, "");
         
       constructor (PUBLIC, {
         this->age = age;
         this->weight = weight;
       }, (int, double), "age", "weight");
        
       virtual (PUBLIC, DYNAMIC, Eat, (void:double), "foodWeight");
          
       method (PUBLIC, DYNAMIC, GetAge, {
         return this->age;
       }, (int:void), NOPARAM);
          
       method (PUBLIC, DYNAMIC, GetWeight, {
         return this->weight;
       }, (double:void), NOPARAM);
     })
     
     class (PUBLIC, DYNAMIC, Cat, {
       inherit(Animal);
     
       override (PUBLIC, DYNAMIC, Eat, {
         this->weight += foodWeight * 0.9;  // Because she drops a lot.
       }, (void:double), "foodWeight");
     })

     /* This outputs the meta data @identity of @Cat, which is "Cat". */
     stringing (ch, of(Cat).identity, {
       ig putchar(ch);
     })
     ig printf(NEWLINE);
     
     Class *c_Cat = new(Cat, 5, 8.8);

     invoke(Cat, Eat, 3);
     
     const double weight = invoke(Cat, GetWeight, NOPARAM);
     
     delete(c_Cat);
     
     RETURN(NormalStatus);
   }
   ```

 - **Incompleted Objects**
 
   Objects like `Array` and `Stack` are commonly used with a type, meaning
   themselves alone are "incompleted".
   
   And when an incompleted object like that need to be defined and used,
   Compound have your back with several options.  Let's take a look.

Rewrite this. -->   
<!--    - **Defining**
   ```C
   ARRAY(YourType);
   ```
   
   - **Usage**
   ```C
   Array(YourType) variable;
   ```
 -->
## Building

> Warning  
>  
> The scripts `build` and `build.sh` may install header files and library output
> into your system.  Make sure you have read the instruction from
> `build.sh`, with or without modifying for your own configuration
> before running.
>  
> Project contributors WILL NOT take any responsibilities for any
> possible damage for abusing our scripts without following given
> guidances.
>  
> YOU HAVE BEEN WARNED.

The script `build` is a convenient shorthand for building project
with default configurations.  

Here is how you use it:

```sh
./build
```

And just for a reminder, that this script installs files into system directories,
so you might want to use it with a root privilege.  

However, if you would like to have more control over compiling this project, you
can choose to use `build.sh`.  There are also many built-in guidances available
in that script to look up.

> The default output for our executable and library file is at `out/bin/`.

### Executable

Compound is originally designed to be a library, meaning there will be no actual
executable files generated.  But it can be achieved by giving an entry along
with library sources.

Here is a demonstration of how you would build an executable file with Compound
using `test.c` included in the repository as an entry.

```sh
./build.sh --sources test.c,src/status.c,src/memory.c,src/arrays.c,src/stacks.c,src/string.c\
           --compiler /usr/bin/tcc\
           --flags -std=c99,-Wall,-Wextra,-Wunsupported,-g\
           --install --complain --dumpenv
```

 - The flag `--sources` specifies source files included sending to the compiler.

 - `--compiler` is optional, and it tells where the compiler user wants
   alternatively is located.

 - `--flags` is also optional.  Its value is passed to the compiler.

 - The flag `--install` is usually used when both header files and shared objects are 
   needed to be installed into the system.  The default locations are 
   `/usr/include/$(ProjectName)` and `/usr/lib`.

 - If you use `--complain`, the output from commands run in each stage is 
   delivered onto the screen.

 - Lastly, when the flag `--dumpenv` is used, there will be a summary of every 
   variable this script applies printed at the beginning.

For more details about `build.sh`, type `build.sh` to check it out.

> Tips & Tricks  
>
> Source specification can be used with shell expansion by an asterisk:
> `--source test.c,src/*`
>
> You can check out the default configuration without doing any compiling work 
> by only giving `--dumpenv` to its parameter list.
>
> If you don't use the `--complain` flag though, there will be an `okay` when 
> commands are successfully executed, or an `failed` with error message for 
> failure.

And finally, you may replace `test.c` with any of your entry source files to 
build your desired executable.

### Shared Object

To build a shared object of Compound, you may use `build.sh` as shown below.

```sh
./build.sh --sources src/status.c,src/memory.c,src/string.c\
           --shared --color --output --complain --dumpenv --install
```

 - `--shared` tells the compiler to treat those source files with `-shared` 
   flag.

 - After giving the `--color` flag, you may have the script output rendered with 
   colors.
 
 - When giving `--output` to the script, it will now tell where the generated 
   output files are placed during this session.

> Tips & Tricks
> 
> With the combination of using `--color` with `--complain`, both the command's 
> and compiler's output will be re-colored.
> 
> When using `--shared` flag, the script will add another stage for `--install` 
> to copy the shared object file into your system (or specified path using 
> `--postinstall-dst`) after all the compiling work are done.

## Contributing

If you have any intention to offer your valuable ideas and/or implementations to 
this project, then we are most delighted to welcome them.  

And while your ideas and/or implementations are being accepted into this 
project's code base, there are some crucial points that you may be interested to 
acknowledge beforehand.

 1. Please ensure that you have read at least once our 
    [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md) before doing anything specific.

 2. If an issue were to be put up, try browsing the previously asked questions and 
    make sure that there are no same or similar ones that were discussed before.

 3. If you don't get an immediate response on whatever you are asking, it 
    doesn't mean your question was ignored.

 4. Remember those rules, please.

And now, welcome!

## Troubleshooting

 1. "*The script `build`/`build.sh` cannot do things right, and it just says 
     `failed` everytime I perform the execution.*"
    
    Try adding `--complain` to the command line, and you'll see the extra message.

 2. "*It kept reporting that I had passed invalid flags when I was just trying
      to set the `--sources`/`--flags`.*"

    These two flags require an input with a special format.  Take a look at this:
      ```sh
      --sources file1,file2,file3
      --flags flag1,flag2,flag3
      ```
    There is *NO SPACE OR TABS* between two files/flags but a comma to separate
    them.

 3. "*I typed every flag along with their values correctly, but I still only get
      same output of helping message again and again.*"

    When a helping screen is presented, that only means one thing: "at least 
    one of your inputs is not recognized."
    
    Try looking closely at the top of every output; there is a brief message that
    tells which input was not accepted.

 4. "*I want ice cream.*"

    ```
         . ,
          *    ,
     ` *~.|,~* '
     '  ,~*~~* `     _
      ,* / \`* '    //
       ,* ; \,O.   //
           ,(:::)=//
          (  `~(###)
           %---'`"y
            \    /
             \  /
            __)(__
           '------`
    ```
