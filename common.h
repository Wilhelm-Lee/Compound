#ifndef COMPOUND_COMMON_h
# define COMPOUND_COMMON_h

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

/**
 * @brief Return an Error Status with given parameter $c as the
 *        comment or description.
 * @return A instance of Error Status customised.
 * @note "error" stands for "Error in Status"
 * @note 'e' stands for "Error"
 * @note 'c' stands for "Comment"
 */
# define error(e, c)  ((Status) {\
  .description = c,\
  .characteristic = e.characteristic,\
  .prev = e.prev\
})

/**
 * @brief Return an Error Status with given parameter $p as the
 *        predecessor.
 * @return A instance of Error Status inherited.
 * @note "extend" stands for "Extend from Predecessor"
 * @note 'i' stands for 'Instance'
 * @note 'p' stands for "Predecessor"
 */
# define extend(i, p)  ((Status)) {\
  .prev = p\
}

# define modify(e, s, c)  ((Status)) {\
  .description = s,\
  .characteristic = c,\
  .prev = e.prev\
}

/** @brief Create a report in place.
 * @return A instance of Status Report customised.
 * @note "stamp" stands for "Report Stamp"
 * @note 'e' stands for "Exception"
 * @note 'c' stands for "Char String of Originator"
 */
# define stamp(e, c)  ((Report) {\
  .stat = e,\
  .originator = c,\
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

// # define sforce(s, k, v)  solve((!status_equal(s, k)), v)

/* Get the literal. */
# define nameof(obj)  #obj

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
  (! ((lf > rt) || ((v <= lf && !inclf) || (v >= rt && !incrt))))

# define ATRANGE(lf, rt, v) \
  (INRANGE(lf, true, rt, true, v)) ? 0 : ((v < lf) ? (v - lf) : (v - rt))

#endif /* NO COMPOUND_COMMON_h */
