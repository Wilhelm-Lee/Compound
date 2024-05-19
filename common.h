#ifndef COMPOUND_COMMON_h
# define COMPOUND_COMMON_h
// # define __DEBUG__  1

# include <stdlib.h>
# include <stdbool.h>

/**
 * @brief Return $n as the return value, once $o is NULL
 * @return given $n as the return value
 * @note "fails" stands for "Fails on Null Check"
 * @note 'o' stands for "Object"
 * @note 'n' stands for "Numeric on Return"
 */
# define fails(o, n)  { if ((o) == NULL) return (n); }

/**
 * @brief Return $e as the return value, once $v equals $e
 * @return given $e as the return value
 * @note "trans" stands for "Transits Error Code to Caller"
 * @note 'v' stands for "Value"
 * @note 'e' stands for "Error Code"
 */
# define trans(v, e)  { if ((v) == (e)) return (e); }

/**
 * @brief Evaluate given statement while the ptr to $s is not NULL
 * @return given $n as the return value
 * @note "state" stands for "Statement Evaluation"
 * @note 's' stands for "Statement"
 * @note 'n' stands for "Numeric on Return"
 */
# define state(s, n)  { if ((s)) return (n); }

/**
 * @brief Evaluate given statement while the ptr to $s is not NULL
 * @return nothing.
 * @note "svoid" stands for "Statement Evaluation in Void"
 * @note 's' stands for "Statement"
 */
# define svoid(s)  { if ((s)) return; }


/* Create a new UnknownStatus on the fly. */
# define unknown(e, c, v)  ((Status) {\
  .value = v,\
  .description = c,\
  .characteristic = STATUS_UNKNOWN,\
  .prev = e.prev\
})

/* Create a new NormalStatus on the fly. */
# define normal(e, c, v)  ((Status) {\
  .value = v,\
  .description = c,\
  .characteristic = STATUS_NORMAL,\
  .prev = e.prev\
})

/* Create a new ErrorStatus on the fly. */
# define error(e, c)  ((Status) {\
  .description = c,\
  .characteristic = STATUS_ERROR,\
  .prev = e.prev\
})

/* Extend the Status chain by giving 'p' for "predecessor"
   and 'c' for "comment/description". */
# define extend(p, c)  ((Status) {\
  .description = c,\
  .characteristic = p.characteristic,\
  .prev = p\
})

/** @brief Create a report in place.
 * @return A instance of Status Report customised.
 * @note "stamp" stands for "Report Stamp"
 * @note 'e' stands for "Exception"
 * @note 'c' stands for "Char String of Originator"
 */
# define stamp(e, c)  ((Report) {\
  .status = e,\
  .initiator = c,\
  .time = time(NULL),\
  .priority = REPORT_SENDING_PRIORITY_NORMAL,\
  .status = REPORT_SENDING_TASK_STATUS_PENDING\
})

/**
 * @brief Another way to handle if statements more cleanly.
 * @note "solve" stands for "Solve with Statements."
 * @note 's' stands for "Statement"
 * @note 'b' stands for "Block of Forks"
 */
# define solve(s, b)  if (s) b

// /**
//  * @brief Forcibly return desired value $v once $s is not $k.
//  * @return $v once state for $s is false.
//  * @note "force" stands for "Forcible value"
//  * @note 's' stands for "Statement"
//  * @note 'k' stands for "Key Value", the value that is targeted to detect.
//  * @note 'v' stands for "Desire Value", the value that desires.
//  */
// # define force(s, k, v)  solve((s) != (k), v)

// # define sforce(s, k, v)  solve((!Status_Equals(s, k)), v)

/* Get the literal. */
# define nameof(obj)  #obj

# define type(T)

/* Only effects when Generic is defined. */
# define Array(T) Array

# define String(T) String

# define cat(s)  {\
  CatlogMsg msg;\
  CatlogMsg_Create(&msg, CATLOG_LEVEL_DEBUG, "CAT", s);\
  CatlogSender sender;\
  CatlogSender_Create(&sender, &msg, stderr);\
  CatlogSender_Send(&sender, "stdout", false);\
}

# define ok(s, b)  {\
  const Status _ = s;\
  if (StatusUtils_IsOkay(_))  b\
}

# define notok(s, b)  {\
  const Status _ = s;\
  if (!StatusUtils_IsOkay(_))  b\
}

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
  (! ((lf > rt) || ((v <= lf && !inclf) || (v >= rt && !incrt))))

# define ATRANGE(lf, rt, v) \
  (INRANGE(lf, true, rt, true, v)) ? 0 : ((v < lf) ? (v - lf) : (v - rt))

# define LITERALISATION_LENGTH_MAXIMUM  0xFFFF

#endif /* NO COMPOUND_COMMON_h */
