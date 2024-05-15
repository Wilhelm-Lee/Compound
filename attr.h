#ifndef COMPOUND_ATTR_H
# define COMPOUND_ATTR_H

# include <Compound/name.h>

/* Only effect (probably) when formal Attribute is defined. 
 * __ATTRIBUTABLE indicates this field is used for further process by Attribute.
 * Or, to put this way, this field has attributions not used so far, but
 * eventually will.
 */
# define __ATTRIBUTABLE
# define __ATTRIBUTABLE__
# define attr(a)

typedef struct _Attribute{
  int serialNo;
  int (*exec)(void *);
  struct _Attribute *prev;
} attr(Executive) Attribute;

/* The base of every attribute.   */
# define Attributable (Attribute){\
  .value = 0,\
  .exec = HANDLER  /* Use report handler here. */\
};

#endif  /* COMPOUND_ATTR_H */
