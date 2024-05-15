# YOU NEED COMPOUND

Generally, Compound is a library for general-purposes programming which you will find essential for higher level of abstraction based on c.

It is worth noticing that, this library is completely aimed for creating another language. That means, every feature update is supposedly to serve the language.

---
## BUILDING

To build Compound, all you need is to type as follow in a terminal:
```shell
cmake .    # Generate "Makefile"
make       # Build
```

> *PLEASE NOTICE*  
> *Compound is originally designed for GNU/Linux and other Unix-like operating systems.*  
> *Support for MS Windows will be possible in the future.*

### Platform specifications
```C
/* Copied on Fri 10 May 16:24:09 CST 2024 */

# if defined __x86_64__ || defined __x86_64  // For 64-bits operating systems.
#  define __COMPOUND_64__
#  define __COMPOUND_PRODUCT__  compound64
# elif defined __i386__ || __i486__ || __i586__ || __i686__ || _X86_ || __X86__  // For 32-bits operating systems.
#  define __COMPOUND_32__
#  define __COMPOUND_PRODUCT__  compound32
# else
#  error Platform not supported.  Please issue this on github.com/Wilhelm-Lee/Compound  --William
# endif
```
Compound, now, roughly divides platform compatibilities into 2 major parts, the 64-bits and the 32-bits.  
This idea is ready for refinements, you can put up such suggestions and they're welcomed.  

## Installation
To install Compound, all you need to do is to run `install`, something like below.
```shell
./install  # (Require root privilege)
```

If you cannot execute ./install script, try to add permission for execution:
```shell
chmod +x ./install  # (Require root privilege)
```
