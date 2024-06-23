#ifndef COMPOUND_STATUS_H
# define COMPOUND_STATUS_H

# include <stdbool.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <threads.h>
# include <time.h>

# include <Compound/common.h>
# include <Compound/platform.h>
# include <Compound/utils.h>

/* Status characteristics */
typedef enum {
  STATUS_UNKNOWN  = -1,
  STATUS_NORMAL   = 0,
  STATUS_ERROR    = 1
} StatusCharacteristics;

typedef enum {
  /* Settlement. */
  ARGUE_RESULT_FINALISED =  0b01,
  ARGUE_RESULT_UNSETTLED = -0b01,

  /* Nature. */
  ARGUE_RESULT_POSITIVE  =  0b10,
  ARGUE_RESULT_NEGATIVE  = -0b10,
} ArgueResult;

/* Indicated the exact location where the "issue" was occurred at. */
typedef struct {
  char *file;
  int line;
  char *func;
} Location;

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

/* Common return type for reporting functions that require to give more
   information about the procedure. */
typedef struct _Status {
  char *identity;
  int value;  /* Traditional returning data "int".  Only used when the function
                 called and received legacy functions that uses "int" as the
                 returning type that wish to have place to hold the value.
                 Otherwise, the function would just return the structure Status. */
  char *description;
  int characteristic;
  Location loc;
  struct _Status *prev;
} Status;

# define DEFSTATUS(i, v, d, c, p)                          \
  static const Status i = {                                \
    .identity = nameof(i),                                 \
    .value = v,                                            \
    .description = d,                                      \
    .characteristic = c,                                   \
    .loc = __GLOBAL__,                                     \
    .prev = (Status *)p                                    \
  }

# define LOCATION_LITERALISE_FORMAT  "at %s:%d, in function `%s\'"

# define STATUS_LITERALISE_FORMAT \
    "%s:  \"%s\"\n\tpredecessor=<%s> value=(%d) characteristic=[%d]\n\t%s\n"

typedef enum {
  REPORT_SENDING_PRIORITY_ALL = 0,  // Highest level; least value.
  REPORT_SENDING_PRIORITY_FATAL,
  REPORT_SENDING_PRIORITY_EXCEPTIONAL,
  REPORT_SENDING_PRIORITY_CRITICAL,
  REPORT_SENDING_PRIORITY_MAJOR,
  REPORT_SENDING_PRIORITY_NORMAL,
  REPORT_SENDING_PRIORITY_MINOR,
  REPORT_SENDING_PRIORITY_DEBUG,
  REPORT_SENDING_PRIORITY_NONE,  // Lowest level, greatest value.
} ReportLevel;

typedef enum {
  REPORT_SENDING_TASK_STATUS_FINISHED = 0,
  REPORT_SENDING_TASK_STATUS_PENDING,
  REPORT_SENDING_TASK_STATUS_PROCEEDING,
  REPORT_SENDING_TASK_STATUS_PAUSED,
  REPORT_SENDING_TASK_STATUS_NOTFOUND
} ReportStatus;

/* "Report" recollects essential informations, included but not limited to
   Status and others for making an report for debugging and such. */
typedef struct {
  Status content;
  char *initiator;
  time_t time;
  ReportLevel level;
  ReportStatus status;
  FILE *dst;  // The destination where the report is sending to.
} Report;

/*
DATETIME [PRIORITY] STATUSNAME (ORIGINATOR): STATUS.DESCRIPTION
    at LOCATION.FILE:LOCATION.LINE, LOCATION.FUNC
    at LOCATION.FILE:LOCATION.LINE, LOCATION.FUNC
    at LOCATION.FILE:LOCATION.LINE, LOCATION.FUNC
    at LOCATION.FILE:LOCATION.LINE, LOCATION.FUNC


Fri 10 May 03:02:37 CST 2024 [URGENT] InvalidParameter (Nullity): Given buffer was unavailable.
    at /external/Documents/Projects/Compound/Status/src/status.c:104, Report_Literalise
    at /external/Documents/Projects/Compound/Status/src/status.c:114, ReportSender_Send
    at /external/Documents/Projects/Compound/Status/src/status.c:69, _throw
!!!!at /external/Documents/Projects/Compound/Array/src/array.c:16, array_create
    at /external/Documents/Projects/Compound/test.c:24, main

*/

// DATETIME [LEVEL] STATUS.IDENTITY (INITIATOR): STATUS.DESCRIPTION
# define REPORT_LITERALISE_FORMAT_HEADER  "%s [%d] %s (%s): %s\n\tat %s:%d, %s\n%s"

// STATUS.IDENTITY, STATUS.PREV.IDENTITY, STATUS.VALUE, STATUS.CHARACTERISTIC,
// FILE, LINE, FUNC
# define REPORT_LITERALISE_FORMAT_DETAIL  "\t%s(%s, %d, %d)  at %s:%d, %s\n"

typedef enum {
  REPORT_RESULT_SUCCEEDED,
  REPORT_RESULT_FAILED,
  REPORT_RESULT_PROGRESSING,
  REPORT_RESULT_PENDING,
} ReportResult;

typedef struct {
  thrd_t thread;
  Report report;
  time_t elapsed;
  ReportResult result;
} ReportSender;

typedef int (*ReportTask)(Report *);
typedef int ReportTaskID;

// typedef thrd_start_t  ArgueStart;

// typedef struct {
//   ArgueStart handler;
//   void *external_param;
// } ArgueStartParam;

// /* Argument levels (qualities) */
// typedef enum {
//   ARGUMENT_NONE = 0,
//   ARGUMENT_MINOR,
//   ARGUMENT_NORMAL,
//   ARGUMENT_MAJOR,
//   ARGUMENT_CRITICAL,
//   ARGUMENT_SEVERE,
//   ARGUMENT_ALL,
// } ArgumentLevel;

// typedef struct {
//   ReportSender stream;
//   ArgueStartParam handler;  // Obsolete?
//   ArgumentLevel level;
//   bool muted;
// } Argument;

// typedef struct {
//   Argument *members;
//   int (*announcer) (Argument);
// } ArgumentAnnouncer;

# define STATUS_BUFFER_MAXIMUM_LENGTH  UINT32_MAX

Status Location_Literalise(Location *inst, char *buff);
bool   Location_Equals(Location lc1, Location lc2);
Status Status_Literalise(Status *inst, char *buff);
Status Status_LiteraliseForReport(Status *inst, char *buff);
bool   Status_Equal(Status *stat1, Status *stat2);
// void   StatusUtils_Dump(Status *inst, Status **store, int idx);
void   StatusUtils_Dump(Status *inst, Status *store);
bool   StatusUtils_HasPrev(Status inst);
bool   StatusUtils_IsOkay(Status inst);
bool   StatusUtils_IsRecursive(Status inst);
int    StatusUtils_Depth(Status *inst);

Status Report_Create(Report *inst, Status *stat, FILE *dest, char *initiator,
                     int priority);
Status Report_CopyOf(Report *inst, Report *other);
Status Report_Literalise(Report *inst, char *buff);
void   Report_Delete(Report *inst);
bool   Report_Equals(Report repo1, Report repo2);

// Status ReportSender_Create(ReportSender *inst, Report *report, thrd_start_t *handler);
Status ReportSender_Create(ReportSender *inst, Report *report);
Status ReportSender_Send(ReportSender *inst, ReportTask task);

// ReportTaskStatus
// ReportSender_GetStatus(ReportSender *inst);

// Status
// arguestarter_create(ArgueStartParam *inst, void *external_param);
// Status
// arguestarter_constr(ArgueStartParam *inst, ArgueStart argue_start,
//                     void *external_param);
// Status
// arguestarter_start(ArgueStartParam *inst);
// bool
// arguestarter_equal(ArgueStartParam *inst1, ArgueStartParam *inst2);
// Status arguestarter_current(void);
// Status arguestarter_sleep(const struct timespec *time_point,
//                           struct timespec *remaining);
// void arguestarter_exit(int code) __attribute__ ((__noreturn__));
// Status arguestarter_join(ArgueStart thrd);


// Status
// argument_create(Argument *inst, ReportSender *streams, ArgueStartParam handler,
//                 int level, bool muted);

// ---------------------ELEMENTARY-------------------------

DEFSTATUS(UnknownStatus, -1, "An unknown status.", STATUS_UNKNOWN, NULL);
DEFSTATUS(NormalStatus, 0, "A normal status.", STATUS_NORMAL, NULL);
DEFSTATUS(ErrorStatus, 1, "An error status.", STATUS_ERROR, NULL);

// ----------------------EXTENDED--------------------------

DEFSTATUS(MemoryViolation, 1,
  "Illegal access on certain memory address.",
  STATUS_ERROR, &ErrorStatus);

DEFSTATUS(NullPointerAccounted, 1,
  "An involving null pointer was not accepted.",
  STATUS_ERROR, &MemoryViolation);

DEFSTATUS(InvalidObject, 1,
  "An invalid object was presented.",
  STATUS_ERROR, &ErrorStatus);

DEFSTATUS(UnavailableObject, 1,
  "An unavailable object was presented.",
  STATUS_ERROR, &ErrorStatus);

DEFSTATUS(InstanceStillAlive, 1,
  "Given instance was yet alive.",
  STATUS_ERROR, &ErrorStatus);

DEFSTATUS(InstanceNotAlive, 1,
  "Given instance for reallocation was not alive.",
  STATUS_ERROR, &ErrorStatus);

DEFSTATUS(InvalidParameter, 1,
  "An invalid parameter was presented.",
  STATUS_ERROR, &InvalidObject);

DEFSTATUS(InsufficientMemory, 1,
  "Not enough room for further memory allocations.",
  STATUS_ERROR, &ErrorStatus);

DEFSTATUS(ArithmeticError, 1,
  "An arithmetic error occurred.",
  STATUS_ERROR, &ErrorStatus);

DEFSTATUS(IntegerOverFlow, 1,
  "An integer had overflowed.",
  STATUS_ERROR, &ArithmeticError);

DEFSTATUS(RuntimeError, 1,
  "A runtime error occurred.",
  STATUS_ERROR, &ErrorStatus);

DEFSTATUS(InstanceCreatingFailure, 1,
  "Cannot create the instance.",
  STATUS_ERROR, &RuntimeError);

DEFSTATUS(ArrayLengthError, 1,
  "Given array length does not meet the requirement.",
  STATUS_ERROR, &ErrorStatus);

DEFSTATUS(VariableFormatMismatch, 1,
  "Given format does not match with given subjects.",
  STATUS_ERROR, &ErrorStatus);

DEFSTATUS(ImprecisionError, 1,
  "Precision was not enough for handling the calculation.",
  STATUS_ERROR, &RuntimeError);


// ---------------------USER DEFINED-----------------------

// The meaning of value is defined by the function itself.
DEFSTATUS(TraditionalFunctionReturn, 0,
  "Function has returned an integer.",
  STATUS_UNKNOWN, &UnknownStatus);

DEFSTATUS(UnavailableInstance, 1,
  "An unavailable instance was given for initialisation.",
  STATUS_ERROR, &NullPointerAccounted);

DEFSTATUS(RecreationOnInstanceStillAlive, 1,
  "Given instance was still alive, yet, was sent for another session of recreation.",
  STATUS_ERROR, &InstanceStillAlive);

DEFSTATUS(UnavailableParameter, 1,
  "An unavailable instance was given as a parameter.",
  STATUS_ERROR, &UnavailableInstance);

DEFSTATUS(InvalidReportTask, 1,
  "An unavailable or illegal report task was given.",
  STATUS_ERROR, &InvalidParameter);

DEFSTATUS(UnableToThrowError, 1,
  "Unable to report an exceptional situation.",
  STATUS_ERROR, &RuntimeError);

DEFSTATUS(ReadWriteError, 1,
  "Error occurred during IO session.",
  STATUS_ERROR, &RuntimeError);

DEFSTATUS(FileNotFound, 1,
  "Target file was unavailable and unable to find.",
  STATUS_ERROR, &ReadWriteError);

DEFSTATUS(InvalidFileName, 1,
  "Given file name was invalid.",
  STATUS_ERROR, &ReadWriteError);

DEFSTATUS(UnavailableFileName, 1,
  "Given file name was unavailable.",
  STATUS_ERROR, &UnavailableObject);

DEFSTATUS(UnavailableFileAccessMode, 1,
  "Given file accessing mode was unavailable.",
  STATUS_ERROR, &UnavailableObject);

DEFSTATUS(InsufficientAccessPermission, 1,
  "Given permission does not suffice to access.",
  STATUS_ERROR, &ReadWriteError);

DEFSTATUS(ReportThrown, 1,
  "This function has thrown a report, following instructions aborted.",
  STATUS_ERROR, &RuntimeError);

DEFSTATUS(ReportMessageTooLong, 1,
  "Given message is too long.",
  STATUS_ERROR, &ArrayLengthError);

DEFSTATUS(MaximumLengthExceeded, 1,
  "Maximum length had exceeded",
  STATUS_ERROR, &ArrayLengthError);

DEFSTATUS(MaximumLiteralisationLengthExceeded, 1,
  "Literalisation was too long",
  STATUS_ERROR, &MaximumLengthExceeded);

DEFSTATUS(UnavailableBuffer, 1,
  "Given buffer was unavailable.",
  STATUS_ERROR, &UnavailableInstance);

DEFSTATUS(InvalidLiteralisingBuffer, 1,
  "Given buffer does not have a good integrity on its length.",
  STATUS_ERROR, &InvalidObject);

DEFSTATUS(NoBytesWereRead, 1,
  "Called function had returned ZERO indicating no bytes were read.",
  STATUS_ERROR, &ReadWriteError);

DEFSTATUS(NoBytesWereWritten, 1,
  "Called function had returned ZERO indicating no bytes were written.",
  STATUS_ERROR, &ReadWriteError);

// DEFSTATUS(ProgrammeConstructionError, 1,
//   "Failed on constructing programme at the entrance.",
//   STATUS_ERROR, &RuntimeError);

// ========================================================

static inline Status PrintStatus(Status s)
{
  /* Literalise. */
  char buff[LITERALISATION_LENGTH_MAXIMUM];
  
  /* Handle returning value. */
  /* No bytes were written to buffer. */
  unsure(Status_Literalise(&s, buff), !_.value, {
    return apply(NoBytesWereWritten);
  })

  /* Output. */
  where(fprintf(stderr, "%s\n", buff), {
    /* Pass on returning value. */
    return apply(value(TraditionalFunctionReturn, _));
  })
}

static inline void PrintStatusDump(Status s)
{
  /* Create dump. */
  const int dump_len = StatusUtils_Depth(&s);
  Status dump[dump_len];
  Status current = s;
  dump[0] = current;  // Put self at leading.
  for (register int i = 1; i < dump_len; i++) {
    // StatusUtils_Dump will only access (storage) the prev.
    // It does not include this status itself.
    StatusUtils_Dump(&current, &dump[i]);
    current = *current.prev;
  }

  /* Output by iterating. */
  for (register int i = 0; i < dump_len; i++) {
    
    unsure(PrintStatus(dump[i]), !_.value, {
      (void)fprintf(stderr, "Unable to literalise.\n");
    })
    
    // seek(PrintStatus(dump[i]), {  // Get returning status.

    //   /* Handle TraditionalFunctionReturn. */
    //   nest(_, __, unsure(__, !__.value, {  // No bytes were written to buffer.
    //     (void)fprintf(stderr, "Unable to literalise.\n");
    //     return;
    //   }));

    //   // Handle abnormal status.
    //   nest(_, __, notok(__, {
    //     /* Output the description as explanation. */
    //     (void)fprintf(stderr, "%s\n", __.description);
    //     return;
    //   }));
    // });
  }
}

// ========================================================

/* Add location parameter requirement in order to give proper information
 * before throwing the report out. */
// # define throw(report)  THROW(report, __HERE__)

/* Useless in C, only for human to see.
   Probably rewrite this in Classify. */
# define throws(e)

// ReportTaskID THROW(Report report, Location loc);
// Report CATCH(ReportTaskID taskid, Status (*handler)());
int HANDLER(void *report);

#endif  /* COMPOUND_STATUS_H */
