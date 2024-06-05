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

/*

{value "description" characteristic prev}

"%d \"%s\" %d %p"

*/

/* line, func, file */
// # define LOCATION_LITERALISE_FORMAT  "at line %d, in %s, %s"

/* file, line, func */
# define LOCATION_LITERALISE_FORMAT  "at %s:%d, in function `%s\'"
# define LOCATION_LITERALISE_FORMAT_LENGTH  20

/* value, description, characteristic, prev */
// # define STATUS_LITERALISE_FORMAT  "%d \"%s\" %d %p"
/* identity, prev->identity, value, characteristic, description, <loc> */
// # define STATUS_LITERALISE_FORMAT  "%s (prev: %s): $=%d @=%d\n\t\"%s\"\n\t%s"

// MaximumLiteralisationLengthExceeded (prev: MaximumLengthExceeded): $=1 @=1

/*
MaximumLengthExceeded:  "Buffer was too long."
  predecessor=<ArrayLengthError> value=(1) characteristic=[1]
  at line 40, in Main, /home/william/Documents/Projects/Compound/test.c

*/

// identity, description, prev->identity, value, characteristic, <loc>
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
} ReportSendingPriority;

typedef enum {
  REPORT_SENDING_TASK_STATUS_FINISHED = 0,
  REPORT_SENDING_TASK_STATUS_PENDING,
  REPORT_SENDING_TASK_STATUS_PROCEEDING,
  REPORT_SENDING_TASK_STATUS_PAUSED,
  REPORT_SENDING_TASK_STATUS_NOTFOUND
} ReportSendingTaskStatus;

/* "Report" recollects essential informations, included but not limited to
   Status and others for making an report for debugging and such. */
typedef struct {
  Status status;
  char *initiator;
  time_t time;
  ReportSendingPriority priority;
  ReportSendingTaskStatus task_status;
  FILE *dest;  // The destination where the report is sending to.
} Report;

/*

DATETIME [PRIORITY] STATUSNAME (ORIGINATOR): STATUS.DESCRIPTION
    at LOCATION.FILE:LOCATION.LINE, LOCATION.FUNC
    at LOCATION.FILE:LOCATION.LINE, LOCATION.FUNC
    at LOCATION.FILE:LOCATION.LINE, LOCATION.FUNC
    at LOCATION.FILE:LOCATION.LINE, LOCATION.FUNC


Fri 10 May 03:02:37 CST 2024 [EXCEPTIONAL] InvalidParameter (Nullity): Given buffer was unavailable.
    at /external/Documents/Projects/Compound/Status/src/status.c:104, Report_Literalise
    at /external/Documents/Projects/Compound/Status/src/status.c:114, ReportSender_Send
    at /external/Documents/Projects/Compound/Status/src/status.c:69, _throw
!!!!at /external/Documents/Projects/Compound/Array/src/array.c:16, array_create
    at /external/Documents/Projects/Compound/test.c:24, main

*/

# define REPORT_LITERALISE_HEADER_FORMAT          "%s [%s] %s (%s): %s"
# define REPORT_LITERALISE_CHAINS_FORMAT          "    at %s:%d, %s"
# define REPORT_LITERALISE_CHAINS_EXCLAIM_FORMAT  "!!!!at %s:%d, %s"

typedef enum {
  REPORT_SENDER_RESULT_FINISHED,
  REPORT_SENDER_RESULT_PROGRESSING,
  REPORT_SENDER_RESULT_PENDING
} ReportSenderResult;

typedef struct {
  thrd_t thread;
  Report *report;  // The report for sending.
  time_t elapsed;  // The individual elapsed time for each report. (Array)
  ReportSenderResult result;
  bool successful;
} ReportSender;

typedef int (*ReportSendingTask)(Report *rep);
typedef int ReportSendingTaskID;

typedef struct {
  ReportSendingTask *tasks;  // Array Ref
  int sendercount;
  int finishedcount;
  int *results;  // Array
} ReportSendingManager;

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
bool   Status_Equal(Status *stat1, Status *stat2);
void   StatusUtils_Dump(Status *inst, Status *store, int idx);
bool   StatusUtils_HasPrev(Status inst);
bool   StatusUtils_IsOkay(Status inst);
bool   StatusUtils_IsValid(Status inst);
bool   StatusUtils_IsRecursive(Status inst);
int    StatusUtils_Depth(Status *inst);

Status Report_Create(Report *inst, Status *stat, FILE *dest, char *initiator,
                     int priority);
Status Report_CopyOf(Report *inst, Report *other);
Status Report_Literalise(Report *inst, char *buff);
void   Report_Delete(Report *inst);
bool   Report_Equals(Report repo1, Report repo2);

Status ReportSender_Create(ReportSender *inst, Report *report);
Status ReportSender_Send(ReportSender *inst, ReportSendingTask task);
// ReportSendingTaskStatus
// ReportSender_GetStatus(ReportSender *inst);

ReportSendingTaskID
ReportSenderManager_AppendTask(ReportSendingManager *inst,
                                ReportSendingTask task);
Status ReportSenderManager_RemoveTask(ReportSendingManager *inst,
                                      ReportSendingTaskID taskid);

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

/*
  typedef struct _Status {
    char *identity;
    int value;
    char *description;
    int characteristic;
    Location loc;
    struct _Status *prev;
  } Status;
*/

static const Status UnknownStatus = {
  .identity = nameof(UnknownStatus),
  .value = -1,
  .description = "An unknown status.",
  .characteristic = STATUS_UNKNOWN,
  .loc = __GLOBAL__,
  .prev = NULL
};

static const Status NormalStatus = {
  .identity = nameof(NormalStatus),
  .value = 0,
  .description = "A normal status.",
  .characteristic = STATUS_NORMAL,
  .loc = __GLOBAL__,
  .prev = NULL
};

static const Status ErrorStatus = {
  .identity = nameof(ErrorStatus),
  .value = 1,
  .description = "An error status.",
  .characteristic = STATUS_ERROR,
  .loc = __GLOBAL__,
  .prev = NULL
};

// ----------------------EXTENDED--------------------------

static const Status MemoryViolation = (Status){
  .identity = nameof(MemoryViolation),
  .value = 1,
  .description = "Illegal access on certain memory address.",
  .characteristic = STATUS_ERROR,
  .loc = __GLOBAL__,
  .prev = (Status *)&ErrorStatus
};

static const Status NullPointerAccounted = (Status){
  .identity = nameof(NullPointerAccounted),
  .value = 1,
  .description = "An involving null pointer was not accepted.",
  .characteristic = STATUS_ERROR,
  .loc = __GLOBAL__,
  .prev = (Status *)&MemoryViolation
};

static const Status InvalidObject = (Status){
  .identity = nameof(InvalidObject),
  .value = 1,
  .description = "An invalid object was presented.",
  .characteristic = STATUS_ERROR,
  .loc = __GLOBAL__,
  .prev = (Status *)&ErrorStatus
};

static const Status UnavailableObject = (Status){
  .identity = nameof(UnavailableObject),
  .value = 1,
  .description = "An unavailable object was presented.",
  .characteristic = STATUS_ERROR,
  .loc = __GLOBAL__,
  .prev = (Status *)&ErrorStatus
};

static const Status InstanceStillAlive = (Status){
  .identity = nameof(InstanceStillAlive),
  .value = 1,
  .description = "Given instance was yet alive.",
  .characteristic = STATUS_ERROR,
  .loc = __GLOBAL__,
  .prev = (Status *)&ErrorStatus
};

static const Status InstanceNotAlive = (Status){
  .identity = nameof(InstanceNotAlive),
  .value = 1,
  .description = "Given instance for reallocation was not alive.",
  .characteristic = STATUS_ERROR,
  .loc = __GLOBAL__,
  .prev = (Status *)&ErrorStatus
};

static const Status InvalidParameter = (Status){
  .identity = nameof(InvalidParameter),
  .value = 1,
  .description = "An invalid parameter was presented.",
  .characteristic = STATUS_ERROR,
  .loc = __GLOBAL__,
  .prev = (Status *)&InvalidObject
};

static const Status InsufficientMemory = (Status){
  .identity = nameof(InsufficientMemory),
  .value = 1,
  .description = "Not enough room for further memory allocations.",
  .characteristic = STATUS_ERROR,
  .loc = __GLOBAL__,
  .prev = (Status *)&ErrorStatus
};

static const Status ArithmeticError = (Status){
  .identity = nameof(ArithmeticError),
  .value = 1,
  .description = "An arithmetic error occurred.",
  .characteristic = STATUS_ERROR,
  .loc = __GLOBAL__,
  .prev = (Status *)&ErrorStatus
};

static const Status IntegerOverFlow = (Status){
  .identity = nameof(IntegerOverFlow),
  .value = 1,
  .description = "An integer had overflowed.",
  .characteristic = STATUS_ERROR,
  .loc = __GLOBAL__,
  .prev = (Status *)&ArithmeticError
};

static const Status RuntimeError = (Status){
  .identity = nameof(RuntimeError),
  .value = 1,
  .description = "A runtime error occurred.",
  .characteristic = STATUS_ERROR,
  .loc = __GLOBAL__,
  .prev = (Status *)&ErrorStatus
};

static const Status ArrayLengthError = (Status){
  .identity = nameof(ArrayLengthError),
  .value = 1,
  .description = "Given array length does not meet the requirement.",
  .characteristic = STATUS_ERROR,
  .loc = __GLOBAL__,
  .prev = (Status *)&ErrorStatus
};

static const Status VariableFormatMismatch = (Status){
  .identity = nameof(VariableFormatMismatch),
  .value = 1,
  .description = "Given format does not match with given subjects.",
  .characteristic = STATUS_ERROR,
  .loc = __GLOBAL__,
  .prev = (Status *)&ErrorStatus
};

static const Status ImprecisionError = (Status){
  .identity = nameof(ImprecisionError),
  .value = 1,
  .description = "Precision was not enough for handling the calculation.",
  .characteristic = STATUS_ERROR,
  .loc = __GLOBAL__,
  .prev = (Status *)&RuntimeError
};

// ---------------------USER DEFINED-----------------------

static const Status UnavailableInstance = (Status){
  .identity = nameof(UnavailableInstance),
  .value = 1,
  .description = "An unavailable instance was given for initialisation.",
  .characteristic = STATUS_ERROR,
  .loc = __GLOBAL__,
  .prev = (Status *)&NullPointerAccounted
};

static const Status RecreationOnInstanceStillAlive = (Status){
  .identity = nameof(RecreationOnInstanceStillAlive),
  .value = 1,
  .description = "Given instance was still alive, yet, was sent for another "
                 "session of recreation.",
  .characteristic = STATUS_ERROR,
  .loc = __GLOBAL__,
  .prev = (Status *)&InstanceStillAlive\
};

static const Status UnavailableParameter = (Status){
  .identity = nameof(UnavailableParameter),
  .value = 1,
  .description = "An unavailable instance was given as a parameter.",
  .characteristic = STATUS_ERROR,
  .loc = __GLOBAL__,
  .prev = (Status *)&UnavailableInstance
};

static const Status InvalidReportTask = (Status){
  .identity = nameof(InvalidReportTask),
  .value = 1,
  .description = "An unavailable or illegal report task was given.",
  .characteristic = STATUS_ERROR,
  .loc = __GLOBAL__,
  .prev = (Status *)&InvalidParameter
};

static const Status UnableToThrowError = (Status){
  .identity = nameof(UnableToThrowError),
  .value = 1,
  .description = "Unable to report an exceptional situation.",
  .characteristic = STATUS_ERROR,
  .loc = __GLOBAL__,
  .prev = (Status *)&RuntimeError
};

static const Status ReadWriteError = (Status){
  .identity = nameof(ReadWriteError),
  .value = 1,
  .description = "Error occurred during IO session.",
  .characteristic = STATUS_ERROR,
  .loc = __GLOBAL__,
  .prev = (Status *)&RuntimeError
};

static const Status FileNotFound = (Status){
  .identity = nameof(FileNotFound),
  .value = 1,
  .description = "Target file was unavailable and unable to find.",
  .characteristic = STATUS_ERROR,
  .loc = __GLOBAL__,
  .prev = (Status *)&ReadWriteError
};

static const Status InvalidFileName = (Status){
  .identity = nameof(InvalidFileName),
  .value = 1,
  .description = "Given file name was invalid.",
  .characteristic = STATUS_ERROR,
  .loc = __GLOBAL__,
  .prev = (Status *)&ReadWriteError
};

static const Status UnavailableFileName = (Status){
  .identity = nameof(UnavailableFileName),
  .value = 1,
  .description = "Given file name was unavailable",
  .characteristic = STATUS_ERROR,
  .loc = __GLOBAL__,
  .prev = (Status *)&UnavailableObject
};

static const Status ReportThrown = (Status){
  .identity = nameof(ReportThrown),
  .value = 1,
  .description = "This function has thrown a report, "
                 "following instructions aborted.",
  .characteristic = STATUS_ERROR,
  .loc = __GLOBAL__,
  .prev = (Status *)&RuntimeError\
};

static const Status ReportMessageTooLong = (Status){
  .identity = nameof(ReportMessageTooLong),
  .value = 1,
  .description = "Given message is too long.",
  .characteristic = STATUS_ERROR,
  .loc = __GLOBAL__,
  .prev = (Status *)&ArrayLengthError
};

static const Status MaximumLengthExceeded = (Status){
  .identity = nameof(MaximumLengthExceeded),
  .value = 1,
  .description = "Buffer was too long.",
  .characteristic = STATUS_ERROR,
  .loc = __GLOBAL__,
  .prev = (Status *)&ArrayLengthError
};

static const Status MaximumLiteralisationLengthExceeded = (Status){
  .identity = nameof(MaximumLiteralisationLengthExceeded),
  .value = 1,
  .description = "Literalisation was too long.",
  .characteristic = STATUS_ERROR,
  .loc = __GLOBAL__,
  .prev = (Status *)&MaximumLengthExceeded
};

static const Status UnavailableBuffer = (Status){
  .identity = nameof(UnavailableBuffer),
  .value = 1,
  .description = "Given buffer was unavailable.",
  .characteristic = STATUS_ERROR,
  .loc = __GLOBAL__,
  .prev = (Status *)&UnavailableInstance
};

static const Status InvalidLiteralisingBuffer = (Status){
  .identity = nameof(InvalidLiteralisingBuffer),
  .value = 1,
  .description = "Given buffer does not have a good integrity on its length.",
  .characteristic = STATUS_ERROR,
  .loc = __GLOBAL__,
  .prev = (Status *)&InvalidObject
};

// ========================================================

/* Throw the report created with $e if $e is abnormal, commented with $c. */
# define ensure(e, c)  {                                   \
  Status stat = e;                                         \
  solve(!(StatusUtils_IsOkay(stat)), {                     \
    Report rep = stamp(error(stat, c), (char *)__func__);  \
    (void)throw(rep);                                      \
    return ReportThrown;                                   \
  })                                                       \
}

/* Throw the report created with $s if $e is abnormal, commented with $c. */
# define match(s, e, c)  {                                 \
  Status stat = s;                                         \
  solve(!(StatusUtils_IsOkay(e)), {                        \
    Report rep = stamp(error(stat, c), (char *)__func__);  \
    (void)throw(rep);                                      \
    return ReportThrown;                                   \
  })                                                       \
}

/* Add location parameter requirement in order to give proper information
 * before throwing the report out. */
# define throw(report)  __throw(report, __HERE__)

/* Useless in C, only for human to see.
   Probably rewrite this in Classify. */
# define throws(e)

ReportSendingTaskID __throw(Report report, Location loc);
Report catch(ReportSendingTaskID taskid);
int HANDLER(void *report);

#endif  /* COMPOUND_STATUS_H */
