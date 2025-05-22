#ifndef COMPOUND_RECOLLECTOR_H
# define COMPOUND_RECOLLECTOR_H

# include "Compound/arrays.h"
# include "Compound/boolean.h"
# include "Compound/status.h"

typedef struct {
  void *obj;
  boolean inuse;
} Mark;

ARRAY(Mark)

typedef struct {
  Array(Mark) marks;
  
} Recollector;



#endif  /* COMPOUND_RECOLLECTOR_H */
