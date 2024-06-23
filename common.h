#ifndef COMPOUND_COMMON_H
# define COMPOUND_COMMON_H

# ifdef __DEBUG__
#  warning DEBUG IS ON
# endif  /* __DEBUG__ */

# include <stdlib.h>
# include <stdbool.h>

# define EMPTY  {0}

/* Get the literal. */
# define nameof(obj)  #obj

/* Return n as the return value, once o is NULL. */
# define nonull(o, n)  { if (!o) return (n); }

/* Return e as the return value, once v equals e. */
# define trans(v, e)  { if ((v) == (e)) return (e); }

/* Evaluate given statement while the ptr to s is not NULL. */
# define state(s, n)  { if ((s)) return (n); }

/* Evaluate given statement while the ptr to s is not NULL. */
# define svoid(s)  { if ((s)) return; }

/* Another way to handle if statements more cleanly. */
# define solve(s, b)  { if (s) b }

/* Handling expression with its result. */
# define when(expr, b)  { int _ = expr; if (_) b }

/* Handling expression with its precalculated result. */
# define where(expr, b)  { int _ = expr; b }

/* Execute b whenever finds s is "okay". */
# define ok(s, b)  { Status _ = s;  if (StatusUtils_IsOkay(_))  b }

/* Execute b whenever finds s is "NOT okay". */
# define notok(s, b)  { Status _ = s;  if (!StatusUtils_IsOkay(_))  b }

/* Return e when passing a failing e commented with c. */
# define fails(e, c)  { notok(e, return apply(annot(_, c));) }

/* Return v when passing a failing e. */
# define vfail(e, v)  { notok(e, return v;) }

/* Execute b for handling UnknownStatus (TraditionalFunctionReturn). */
# define unsure(s, expr, b)  { Status _ = s; \
  if (_.characteristic < 0 && (expr))  b }

/* Execute b whatsoever with s stored. */
# define seek(s, b)  { Status _ = s;  b }

/* Combinates seek and solve. */
# define settle(e, s, b)  seek(e, solve(s, b))

/* Clone a new varaible "v2" with "v1". */
# define clone(v1, v2)  __typeof__(v1) v2 = v1;

/* Allows different macros using "_" nested with each other. */
# define nest(v1, v2, b)  { clone(v1, v2) b }

// # define lambda(param, body, capfmt, ...)  {\
//   /* Duplicate everything from cap. */\
//   va_list ptr;\
//   va_start(ptr, capfmt);\
//   __typeof__(ptr) \
//   va_end(ptr);\
// }

/* Create a new UnknownStatus on the fly. */
# define unknown(e, c, v)  ((Status) {\
  .identity = nameof(e),\
  .value = v,\
  .description = c,\
  .characteristic = STATUS_UNKNOWN,\
  .loc = e.loc,\
  .prev = e.prev\
})

/* Create a new NormalStatus on the fly. */
# define normal(e, c)  ((Status) {\
  .identity = nameof(e),\
  .value = 0,\
  .description = c,\
  .characteristic = STATUS_NORMAL,\
  .loc = e.loc,\
  .prev = e.prev\
})

/* Create a new ErrorStatus on the fly. */
# define error(e, c)  ((Status) {\
  .identity = nameof(e),\
  .value = e.value,\
  .description = c,\
  .characteristic = STATUS_ERROR,\
  .loc = e.loc,\
  .prev = e.prev\
})

/* Extend the Status chain by giving 'p' for "predecessor" and 'e' for "Eval-Status". */
# define extend(p, e)  ((Status) {\
  .identity = e.identity,\
  .value = p.value,\
  .description = e.description,\
  .characteristic = p.characteristic,\
  .loc = p.loc,\
  .prev = (Status *)&p\
})

# define value(e, v)  ((Status) {\
  .identity = e.identity,\
  .value = v,\
  .description = e.description,\
  .characteristic = e.characteristic,\
  .loc = e.loc,\
  .prev = (Status *)e.prev\
})

# define apply(e)  ((Status) {\
  .identity = e.identity,\
  .value = e.value,\
  .description = e.description,\
  .characteristic = e.characteristic,\
  .loc = __HERE__,\
  .prev = (Status *)e.prev\
})

// Reannotate for e.
# define annot(e, c)  ((Status) {\
  .identity = e.identity,\
  .value = e.value,\
  .description = c,\
  .characteristic = e.characteristic,\
  .loc = e.loc,\
  .prev = (Status *)e.prev\
})

/* Create a report on the fly. */
# define stamp(e, ini)  ((Report) {\
  .content = e,\
  .initiator = ini,\
  .time = time(NULL),\
  .level = REPORT_SENDING_PRIORITY_NORMAL,\
  .status = REPORT_SENDING_TASK_STATUS_PENDING,\
  .dst = stdout\
})

# define cat(s)  {\
  CatlogMsg msg;\
  CatlogMsg_Create(&msg, CATLOG_LEVEL_DEBUG, "CAT", s);\
  CatlogSender sender;\
  CatlogSender_Create(&sender, &msg, stderr);\
  CatlogSender_Send(&sender);\
}

# define strnil(s)  (!s ? ("(null)") : s)

# define type(T)

/* Only effects when Generic is defined. */
# define Array(T) Array

# define String(T) String

typedef enum {
  COMMON_ERROR_NULLPTR = 1,
  COMMON_ERROR_INVALID_ARGUMENT,
  COMMON_ERROR_OUT_OF_MEMORY,
  COMMON_ERROR_ARITHMETIC,
} CommonError;

typedef struct {
  int x;
  int y;
} Coordination;

typedef struct {
  Coordination start;
  Coordination end;
} Selection;

typedef struct {
  void *addr;
  size_t sz;
} MemoryInst;

typedef Coordination ArrayIndexerRange;
typedef bool _Bit;

# define WHICH_MIN(a, b)  

# define INRANGE(lf, inclf, rt, incrt, v) \
  (!((lf > rt) || ((v <= lf && !inclf) || (v >= rt && !incrt))))

# define ATRANGE(lf, rt, v) \
  (INRANGE(lf, true, rt, true, v)) ? 0 : ((v < lf) ? (v - lf) : (v - rt))

# define LITERALISATION_LENGTH_MAXIMUM  0xFFFFL

/* Only effect (probably) when formal Attribute is defined. 
 * __ATTRIBUTABLE indicates this field is used for further process by Attribute.
 * Or, to put this way, this field has attributions not used so far, but
 * eventually will.
 */
# define __ATTRIBUTABLE
# define __ATTRIBUTABLE__
# define attr(a)

#endif /* COMPOUND_COMMON_H */
