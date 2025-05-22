#ifndef COMPOUND_RECOLLECTOR_H
# define COMPOUND_RECOLLECTOR_H

# include "../inc/arrays.h"
# include "../inc/boolean.h"
# include "../inc/status.h"

typedef struct {
  void *obj;
  boolean inuse;
} Mark;

ARRAY(Mark)

typedef struct {
  Array(Mark) marks;
  
} Recollector;



#endif  /* COMPOUND_RECOLLECTOR_H */
