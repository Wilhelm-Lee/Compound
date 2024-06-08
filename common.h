#ifndef COMPOUND_COMMON_h
# define COMPOUND_COMMON_h
// # define __DEBUG__  1

# include <stdlib.h>
# include <stdbool.h>

/* Get the literal. */
# define nameof(obj)  #obj

/* Return $n as the return value, once $o is NULL. */
# define fails(o, n)  { if (!o) return (n); }

/* Return $e as the return value, once $v equals $e. */
# define trans(v, e)  { if ((v) == (e)) return (e); }

/* Evaluate given statement while the ptr to $s is not NULL. */
# define state(s, n)  { if ((s)) return (n); }

/* Evaluate given statement while the ptr to $s is not NULL. */
# define svoid(s)  { if ((s)) return; }

/* Another way to handle if statements more cleanly. */
# define solve(s, b)  { if (s) b }

/* Handling expression with its result. */
# define when(expr, b)  { int _ = expr; if (expr) b }

/* Handling expression with its calculated result. */
# define where(expr, b)  { int _ = expr; b }

# define ok(s, b)  { Status _ = s;  if (StatusUtils_IsOkay(_))  b }

# define notok(s, b)  { Status _ = s;  if (!StatusUtils_IsOkay(_))  b }

# define seek(s, b)  { Status _ = s;  b }

/* Create a new UnknownStatus on the fly. */
# define unknown(e, c, v)  ((Status) {\
  .identity = e.identity,\
  .value = v,\
  .description = c,\
  .characteristic = STATUS_UNKNOWN,\
  .loc = __HERE__,\
  .prev = e.prev\
})

/* Create a new NormalStatus on the fly. */
# define normal(e, c)  ((Status) {\
  .identity = e.identity,\
  .value = 0,\
  .description = c,\
  .characteristic = STATUS_NORMAL,\
  .loc = __HERE__,\
  .prev = e.prev\
})

/* Create a new ErrorStatus on the fly. */
# define error(e, c)  ((Status) {\
  .identity = e.identity,\
  .value = e.value,\
  .description = c,\
  .characteristic = STATUS_ERROR,\
  .loc = __HERE__,\
  .prev = e.prev\
})

/* Extend the Status chain by giving 'p' for "predecessor" and 'e' for "Eval-Status". */
# define extend(p, e)  ((Status) {\
  .identity = e.identity,\
  .value = p.value,\
  .description = e.description,\
  .characteristic = p.characteristic,\
  .loc = __HERE__,\
  .prev = (Status *)&p\
})

# define value(e, v)  ((Status) {\
  .identity = e.identity,\
  .value = v,\
  .description = e.description,\
  .characteristic = e.characteristic,\
  .loc = __HERE__,\
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

/* Create a report in place with $e for base and $c for initiator literal. */
# define stamp(e, c)  ((Report) {\
  .status = e,\
  .initiator = c,\
  .time = time(NULL),\
  .priority = REPORT_SENDING_PRIORITY_NORMAL,\
  .taskprint_status = REPORT_SENDING_TASK_STATUS_PENDING\
})

# define cat(s)  {\
  CatlogMsg msg;\
  CatlogMsg_Create(&msg, CATLOG_LEVEL_DEBUG, "CAT", s);\
  CatlogSender sender;\
  CatlogSender_Create(&sender, &msg, stderr);\
  CatlogSender_Send(&sender);\
}

// /**
//  * @brief Forcibly return desired value $v once $s is not $k.
//  * @return $v once state for $s is false.
//  * @note "force" stands for "Forcible value"
//  * @note 's' stands for "Statement"
//  * @note 'k' stands for "Key Value", the value that is targeted to detect.
//  * @note 'v' stands for "Desire Value", the value that desires.
//  */
// # define force(s, k, v)  solve((s) != (k), v)

// # define sforce(s, k, v)  solve((!Status_Equal(s, k)), v)

// # define print_status(s)  {\
//   Status _ = s;\
//   char buff[LITERALISATION_LENGTH_MAXIMUM];\
//   notok(Status_Literalise(&_, buff), {\
//     (void)printf("Failed to literalise a status\n");\
//   });\
//   (void)printf("%s\n", buff);\
// }

// # define print_status(s)  {\
//   char buff[LITERALISATION_LENGTH_MAXIMUM];\
//   (void)Status_Literalise(&s, buff);\
//   (void)fprintf(stderr, "%s\n", buff);\
// }

// # define print_statusdump(s)  {\
//   Status _ = s;\
//   const int dump_len = StatusUtils_Depth(&_);\
//   Status dump[dump_len];\
//   StatusUtils_Dump(&_, dump, 0);\
//   for (register int i = 0; i < dump_len; i++) {\
//     /* TODO(william): Replace following line with coloured-term-output function. */\
//     (void)printf("\e[1m[%d/%d]\e[0m\n", i, dump_len - 1);\
//     print_status(dump[i]);\
//   }\
// }

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

#endif /* NO COMPOUND_COMMON_h */
