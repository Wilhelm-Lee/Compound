#ifndef COMPOUND_ATTR_H
# define COMPOUND_ATTR_H

# include <Compound/name.h>

typedef struct _Attribute {
  Name identity;  // Numeral accumulative, not literal descriptive.
  int (*exec)(void *);
  struct _Attribute *prev;
} attr(Executive) Attribute;

/* The base of every attribute.   */
# define Attributable (Attribute){\
  .value = 0,\
  .exec = HANDLER  /* Use report handler here. */\
};

#endif  /* COMPOUND_ATTR_H */
