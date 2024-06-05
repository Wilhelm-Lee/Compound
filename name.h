#ifndef COMPOUND_NAME_H
# define COMPOUND_NAME_H

# include <Compound/common.h>
# include <Compound/status.h>
# include <Compound/array.h>

//  Name, is an interger.
//  It contains 64 digits, each bit representing 63 possible characters.
//  Possible characters:  [a-zA-Z_]
//  At the head of each Name, 8 pilots stores for its type.
//  In total, ((26*2+10+1)^63)*8
//              = 63^63*8
//              = 1.82618429078e+114
//  Nice.  :3
//  William, Sun 12 May, 2024, 02:29:36

# define NAME_LENGTH_MAXIMUM    64
# define NAME_LEGAL_CHARACTERS  "abcdefghijklmnopqrstuvwxyz"\
                                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"\
                                "1234567890"

typedef struct {
  char *identifier;
  Array(int) attributions;
} Name;

static const Name NullName = {
  .identifier =
      "_000000000000000000000000000000000000000000000000000000000000000",
  .attributions = NULL
};

// typedef struct {
//   Name value;
//   Name idx;
//   Name *occupied;
// } NameScope;  // Size: 8+8+8 =8*3 =24 Bytes (x64)

/*
 * Example:
 *   Var variable_from_namescope_Utils_in_class_Calculation_in_function_C;
 *   // Not enough room for representation.
 *   // Must use another more efficient method to store the name.
 * Become (roughly):
 *   Var v0000000000000000000000000000000000000000000000000000000000001g3;
 *   Var v1g3;  // Trimmed.
 *   PILOT: v, f, c, e, i, s, u, a
 *       v: Variable
 *       f: Function
 *       c: Class
 *       e: Enumeration
 *       i: Interface
 *       s: Struct
 *       u: Union
 *       a: Attribution
 *   ID: [a-zA-Z0-9_]
 *   MAXIMUM STORING SIZE: 63 Bytes
 *   TOTAL POSSIBILITIES: ((26*2+10+1)^63)*8 = 1.82618429078e+114
 *   Definitely enough.
 *
 *  Please note, numeral symbols cannot lead the name.
 *  They must have indexer that greater than ZERO.
 */

Status Name_CountDown(Name *inst);
Status Name_CountDownFor(Name *inst, Name amount);
Status Name_CountUp(Name *inst);
Status Name_CountUpFor(Name *inst, Name amount);
Status Name_EmptyName(Name *inst);
Status Name_FormatInflate(Name *inst);
Status Name_FormatTrim(Name *inst);
Name   Name_CalcNameArrayLength(Name **arr);
bool   Name_IsValid(Name *inst);
int    Name_Compare(Name *a, Name *b) throws(InvalidName);

static Status NameOverFlowError = {
  .description = "An overflow occurred while calculating Name.",
  .characteristic = STATUS_ERROR,
  .prev = &ArithmeticError
};

static Status InvalidName = {
  .description = "Given Name was invalid.",
  .characteristic = STATUS_ERROR,
  .prev = &InvalidParameter
};

#endif  /* COMPOUND_NAME_H */
