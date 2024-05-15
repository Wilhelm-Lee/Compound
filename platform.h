#ifndef COMPOUND_PLATFORM_H
# define COMPOUND_PLATFORM_H

# if defined __x86_64__ || defined __x86_64
#  define __COMPOUND_64__
#  define __COMPOUND_PRODUCT__  compound64
# elif defined __i386__ || __i486__ || __i586__ || __i686__ || _X86_ || __X86__
#  define __COMPOUND_32__
#  define __COMPOUND_PRODUCT__  compound32
# else
#  error Platform not supported.  Please issue this on github.com/Wilhelm-Lee/Compound  --William
# endif

#endif  /* COMPOUND_PLATFORM_H */
