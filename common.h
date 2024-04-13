#ifndef COMMON_H
# define COMMON_H

# include <stdlib.h>
# include <stdbool.h>

# define _buffer_definition_without_assignment(type, length)                  \
         struct { type data[(length)]; const int len; }

# define Buffer(type, length, var)                                            \
         _buffer_definition_without_assignment(type, length)                  \
         var = { .data = {}, .len = length }

# define Array(type, length, var)  type var[(length)]

/**
 * @brief Return $n as the return value, once $o is NULL
 * @return given $n as the return value
 * @note "fails" stands for "Fails on Null Check"
 * @note 'o' stands for "Object"
 * @note 'n' stands for "Numeric on Return"
 */
# define fails(o, n) {if ((o) == NULL) return (n);}

/**
 * @brief Return $e as the return value, once $v equals $e
 * @return given $e as the return value
 * @note "trans" stands for "Transits Error Code to Caller"
 * @note 'v' stands for "Value"
 * @note 'e' stands for "Error Code"
 */
# define trans(v, e) {if ((v) == (e)) return (e);}

///**
// * @brief Evaluate given statement while the ptr to $s is not NULL
// * @return given $n as the return value
// * @note "state" stands for "Statement Evaluation"
// * @note 's' stands for "Statement"
// * @note 'n' stands for "Numeric on Return"
// */
//# define state(s, n) {if ((#s)) return (n);}

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

#endif /* NO COMMON_H */
