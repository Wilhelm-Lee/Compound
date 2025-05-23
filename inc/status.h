/*
 * This file is part of Compound library.
 * Copyright (C) 2024-TODAY  William Lee
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 * 
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, see
 * <https://www.gnu.org/licenses/>.
 */

#ifndef COMPOUND_STATUS_H
# define COMPOUND_STATUS_H

# include <stdint.h>
# include <stdio.h>
# include <string.h>

# include "boolean.h"
# include "common.h"
# include "platform.h"
# include "stack.h"

/* Status characteristics */
typedef enum {
  STATUS_UNKNOWN  = -1,
  STATUS_NORMAL   = 0,
  STATUS_ERROR    = 1
} StatusCharacteristics;

# define __HERE__  (Location){                             \
  .file = (char *)__FILE__,                                \
  .line = __LINE__,                                        \
  .func = (char *)__func__                                 \
}

# define __GLOBAL__ (Location){                            \
  .file = (char *)__FILE__,                                \
  .line = __LINE__,                                        \
  .func = (char *)"(GLOBAL)"                               \
}

/* Indicated the exact location where the "issue" was occurred at. */
typedef struct {
  char *file;
  int line;
  char *func;
} Location;

/* Common return type for reporting functions that require to give more
   information about the procedure. */
typedef struct Status {
  char *identity;
  int value;  /* Traditional returning data "int". Only used when the function
                 called and received legacy functions that uses "int" as the
                 returning type that wish to have place to hold the value.
                 Otherwise, the function would just return the structure Status. */
  char *description;
  int characteristic;
  Location location;
  struct Status *prev;
} Status;

typedef struct _StatusStack {
  Status *data;
  size_t length;
  size_t offset;
} StatusStack;
Status StatusStack_Create(StatusStack *inst, const size_t length);
Status StatusStack_CopyOf(StatusStack *inst, const StatusStack other);
Status StatusStack_Delete(StatusStack *inst);
Status StatusStack_Push(StatusStack *inst, const Status elem);
Status StatusStack_Pop(StatusStack *inst);
void StatusStack_PopAll(StatusStack *inst);
inline Status *StatusStack_GetTop(StatusStack *inst);
size_t StatusStack_Literalise(char *buff, void *object, int countdown,
                              struct StatusStack_lit_callback *callback,
                              const char *format, ...);
void StatusStackLiteralise_SetFormat(const char *format);
;
static StatusStack *GlobalStatusStack = ((void *)0);
static inline void StatusStack_SetGlobal(StatusStack *stk) {
  GlobalStatusStack = stk;
};
typedef struct StatusStack_lit_callback {
  size_t offset;
  size_t (*Literalise)(char *buff, void *object, int countdown,
                       struct StatusStack_lit_callback *callback,
                       const char *format, ...);
  char *format;
  struct LiteralisationCallback *next;
} StatusStack_lit_callback;
// size_t StatusStack_Literalise(char *buff, void *object, int countdown,
//                               struct StatusStack_lit_callback *callback,
//                               const char *format, ...);
// void StatusStackLiteralise_SetFormat(const char *format);
// static char *StatusStack_LITERALISATION_FORMAT = ("[>%s]");
// static StatusStackLiteralisationCallback StatusStack_lit_callback = {
//     .offset = 0,
//     .Literalise = StatusStack_Literalise,
//     .format = ("[>%s]"),
//     .next = ((void *)0)};
// ;

# define STATUS(i, v, d, c, p)                             \
  static Status i = {                                      \
    .identity = nameof(i),                                 \
    .value = v,                                            \
    .description = d,                                      \
    .characteristic = c,                                   \
    .location = __GLOBAL__,                                \
    .prev = (Status *)p                                    \
  }

# define LOCATION_LITERALISE_FORMAT  "in file %s, line %d, function %s"

# define STATUS_REGISTRY_BUFFER_MAXIMUM_LENGTH  0xFFFF

/* Returns bytes written. */
boolean   Location_Equals(const Location loc1, const Location loc2);
/* Returns bytes written. */
boolean   Status_Equal(const Status stat1, const Status stat2);
boolean   Status_Match(const Status stat1, const Status stat2);
boolean   Status_Belong(const Status stat1, const Status stat2);
void   Status_Dump(const Status inst, Status *store);
boolean   Status_HasPrev(const Status inst);
boolean   Status_IsOkay(const Status inst);
boolean   Status_IsRecursive(const Status inst);
int    Status_Depth(const Status inst);

// LITERALISATION(Location, "at %s:%d, in function `%s\'");
// LITERALISATION(Status, "%s:  \"%s\"\n\tpredecessor=<%p> value=(%d) characteristic=(%d)\n\t%s");
// FORMAT(Status, );

// ---------------------ELEMENTARY-------------------------

STATUS(UnknownStatus, -1, "Status Unknown.", STATUS_UNKNOWN, &UnknownStatus);
STATUS(NormalStatus, 0, "Status Normal.", STATUS_NORMAL, &NormalStatus);
STATUS(ErrorStatus, 1, "Status Error.", STATUS_ERROR, &ErrorStatus);
STATUS(ErrNo, -1, "ErrNo.", STATUS_ERROR, &ErrNo);

// ----------------------EXTENDED--------------------------

STATUS(Failure, 1,
          "Given status did not pass the failure detection.",
          STATUS_ERROR, &Failure);

STATUS(MemoryViolation, 1,
          "Illegal access on certain memory address.",
          STATUS_ERROR, &ErrorStatus);

STATUS(NullPointerInvolved, 1,
          "Trying to operate with a null pointer.",
          STATUS_ERROR, &ErrorStatus);

STATUS(InvalidObject, 1,
          "An invalid object was presented.",
          STATUS_ERROR, &ErrorStatus);

STATUS(UnavailableObject, 1,
          "An unavailable object was presented.",
          STATUS_ERROR, &ErrorStatus);

STATUS(InvalidParameter, 1,
          "An invalid parameter was presented.",
          STATUS_ERROR, &InvalidObject);

STATUS(InvalidSize, 1,
          "Given size is either invalid for being ZERO, or, invalid according to certain criteria.",
          STATUS_ERROR, &InvalidObject);

STATUS(InvalidMaximumSize, 1,
          "Given maximum size is zero.",
          STATUS_ERROR, &InvalidSize);

STATUS(InsufficientMemory, 1,
          "Not enough room for further memory allocations.",
          STATUS_ERROR, &ErrorStatus);

STATUS(ArithmeticError, 1,
          "An arithmetic error occurred.",
          STATUS_ERROR, &ErrorStatus);

STATUS(IntegerOverFlow, 1,
          "An integer had overflowed.",
          STATUS_ERROR, &ArithmeticError);

STATUS(InvalidOperation, 1,
          "An invalid operation was detected.",
          STATUS_ERROR, &ErrorStatus);

STATUS(RuntimeError, 1,
          "A runtime error had occurred.",
          STATUS_ERROR, &ErrorStatus);

STATUS(UnexpectedSituation, 1,
          "This thread had ran into an unexpected situation.",
          STATUS_ERROR, &RuntimeError);

STATUS(InstanceStillAlive, 1,
          "Given instance was still alive.",
          STATUS_ERROR, &RuntimeError);

STATUS(InstanceNotAlive, 1,
          "Given instance was not alive.",
          STATUS_ERROR, &RuntimeError);

STATUS(SourceNotAlive, 1,
          "Given source was not alive.",
          STATUS_ERROR, &InstanceNotAlive);

STATUS(StoreNotAlive, 1,
          "Given object for storing was not alive.",
          STATUS_ERROR, &InstanceNotAlive);

STATUS(StoreStillAlive, 1,
          "Given object for storing was still alive.",
          STATUS_ERROR, &InstanceStillAlive);

STATUS(InvalidOperationBetweenAliveAndNonAlive, 1,
          "Given two instances were incompatible with each other for any operation.",
          STATUS_ERROR, &InvalidOperation);

STATUS(InstanceCreatingFailure, 1,
          "Cannot create the instance.",
          STATUS_ERROR, &RuntimeError);

STATUS(InvalidArrayLength, 1,
          "Given length is invalid.",
          STATUS_ERROR, &InvalidObject);

STATUS(InvalidStackLength, 1,
          "Given length is invalid.",
          STATUS_ERROR, &InvalidObject);

STATUS(IndexOutOfBound, 1,
          "Accessing elements out of bound.",
          STATUS_ERROR, &MemoryViolation);

STATUS(VariableFormatMismatch, 1,
          "Given format does not match with given subjects.",
          STATUS_ERROR, &ErrorStatus);

STATUS(ImprecisionError, 1,
          "Precision was not enough for handling the calculation.",
          STATUS_ERROR, &RuntimeError);

// The value is the returning one from the function called.
STATUS(TraditionalFunctionReturn, 0,
          "Function has returned an integer.",
          STATUS_UNKNOWN, &UnknownStatus);

STATUS(RecursionTerminated, 0,
          "Function with recursive calling has terminated.",
          STATUS_UNKNOWN, &UnknownStatus);

STATUS(UnavailableInstance, 1,
          "An unavailable instance was presented.",
          STATUS_ERROR, &NullPointerInvolved);

STATUS(UnavailableIdentifier, 1,
          "Given identifier was unavailable.",
          STATUS_ERROR, &UnavailableObject);

STATUS(UnavailableParameter, 1,
          "An unavailable instance was given as a parameter.",
          STATUS_ERROR, &UnavailableInstance);

STATUS(UnavailableSource, 1,
          "Given source is unavailable.",
          STATUS_ERROR, &UnavailableParameter);

STATUS(UnavailableStore, 1,
          "Given object for storing was unavailable.",
          STATUS_ERROR, &UnavailableParameter);

STATUS(InvalidReportTask, 1,
          "An unavailable or illegal report task was given.",
          STATUS_ERROR, &InvalidParameter);

STATUS(UnableToThrowError, 1,
          "Unable to report an exceptional situation.",
          STATUS_ERROR, &RuntimeError);

STATUS(ReadWriteError, 1,
          "Error occurred during IO session.",
          STATUS_ERROR, &RuntimeError);

STATUS(FileNotFound, 1,
          "Target file was unavailable and unable to find.",
          STATUS_ERROR, &ReadWriteError);

STATUS(InvalidFileName, 1,
          "Given file name was invalid.",
          STATUS_ERROR, &ReadWriteError);

STATUS(UnavailableFileName, 1,
          "Given file name was unavailable.",
          STATUS_ERROR, &UnavailableObject);

STATUS(UnavailableFileAccessMode, 1,
          "Given file accessing mode was unavailable.",
          STATUS_ERROR, &UnavailableObject);

STATUS(CannotOpenFile, 1,
          "Failed to open file.",
          STATUS_ERROR, &ReadWriteError);

STATUS(InsufficientAccessPermission, 1,
          "Given permission does not suffice to access.",
          STATUS_ERROR, &ReadWriteError);

STATUS(ReportThrown, 1,
          "This function has thrown a report, following instructions aborted.",
          STATUS_ERROR, &RuntimeError);

STATUS(ReportMessageTooLong, 1,
          "Given message is too long.",
          STATUS_ERROR, &InvalidArrayLength);

STATUS(MaximumLengthExceeded, 1,
          "Maximum length had exceeded.",
          STATUS_ERROR, &InvalidArrayLength);

STATUS(MaximumLengthIsZero, 1,
          "Maximum length limit is zero.",
          STATUS_ERROR, &ErrorStatus);

STATUS(MaximumLiteralisationLengthExceeded, 1,
          "Literalisation was too long.",
          STATUS_ERROR, &MaximumLengthExceeded);

STATUS(UnavailableBuffer, 1,
          "Given buffer was unavailable.",
          STATUS_ERROR, &UnavailableInstance);

STATUS(InvalidLiteralisingBuffer, 1,
          "Given buffer does not have a good integrity on its length.",
          STATUS_ERROR, &InvalidObject);

STATUS(NoBytesWereRead, 1,
          "Called function had returned ZERO indicating no bytes were read.",
          STATUS_ERROR, &ReadWriteError);

STATUS(NoBytesWereWritten, 1,
          "Called function had returned ZERO indicating no bytes were written.",
          STATUS_ERROR, &ReadWriteError);

STATUS(StringConversionPrecisionError, 1,
          "Unpreventable precision loss was found during conversion between string "
          "and raw data.", STATUS_ERROR, &ImprecisionError);

STATUS(UnsupportedEncoding, 1,
          "Given encoding is not supported.",
          STATUS_ERROR, &InvalidObject);

STATUS(FunctionNotDeployed, 1,
          "The function called have not been deployed.",
          STATUS_ERROR, &InvalidOperation);

STATUS(ReadonlyTarget, 1,
          "Given target does not support such modifications.",
          STATUS_ERROR, &InvalidOperation);

STATUS(IncorrectLiveness, 1,
          "Given instance does not have required liveness for further operations.",
          STATUS_ERROR, &InvalidObject);

STATUS(StackError, 1,
          "Cannot perform operation on stack.",
          STATUS_ERROR, &InvalidOperation);

STATUS(StackFull, 1,
          "Stack is full, cannot push.",
          STATUS_ERROR, &StackError);

STATUS(StackEmpty, 1,
          "Stack is empty, cannot pop.",
          STATUS_ERROR, &StackError);

STATUS(InvalidArrayData, 1,
          "Array data cannot be null.",
          STATUS_ERROR, &InvalidObject);

// ========================================================
static void PrintStatus(const Status stat)
{
  ignore printf("at %s:%d, in function `%s\':"NEWLINE
                "\t[%s]<%p>(%d)(%d): %s"NEWLINE,
                stat.location.file, stat.location.line, stat.location.func,
                stat.identity, stat.prev, stat.value, stat.characteristic,
                stat.description);

  /* Handle returning value. */
  /* No bytes were written to buffer. */
  // zero(Status_Literalise(s, buff), {
  //   return apply(NoBytesWereWritten);
  // })
  
  // (void)Literalise(buff, Status,
  //                  stat.identity, stat.description,
  //                  stat.prev->identity, stat.value,
  //                  stat.characteristic);

  /* Output. */
  // where(fprintf(stderr, "%s\n", buff), {
  //   /* Pass on returning value. */
  //   return apply(value(TraditionalFunctionReturn, _));
  // })
}

static void PrintStatusDump(const Status stat)
{
  /* Create dump. */
  /* Calculate depth for dumping. */
  const int dump_len = Status_Depth(stat);

  /* Skip when "s" is either unavailable or is at the bottom of status stack. */
  if (dump_len == -1) {
    PrintStatus(stat);
    return;
  }

  Status dump[dump_len];
  Status current = stat;
  dump[0] = current;  // Put self at leading.
  for (register int i = 1; i < dump_len; i++) {
    // Status_Dump will only access (storage) the prev.
    // It does not include this status itself.
    Status_Dump(current, &dump[i]);

    current = *current.prev;
  }

  /* Output by iterating. */
  for (register int i = 0; i < dump_len; i++) {
    /* Print out indexer. */
    // (void)printf("\e[1m[%d/%d]\e[0m", (dump_len - i), dump_len);

    /* Print dumped status. */
    // zero(PrintStatus(dump[i]).value, {
    //   (void)fprintf(stderr, "Unable to literalise.\n");
    // })
    PrintStatus(dump[i]);
  }
}

#endif  /* COMPOUND_STATUS_H */
