#ifndef COMPOUND_STATUS_H
# define COMPOUND_STATUS_H

# include <stdbool.h>
# include <stdint.h>
# include <stdlib.h>
# include <string.h>
# include <threads.h>
# include <time.h>
# include <stdio.h>
# include <math.h>

# include <Compound/common.h>
# include <Compound/utils.h>
# include <Compound/platform.h>

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
    .file = (char *)__FILE__,                              \
    .line = __LINE__,                                      \
    .func = (char *)__func__                               \
}

/* Common return type for reporting functions that require to give more
   information about the procedure. */
typedef struct _Status {
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

# ifdef __COMPOUND_32__
#  define STATUS_LITERALISE_LENGTH(stat) \
     (utils_calc_digits(stat.value) + strlen(stat.description) + 2 + \
          utils_calc_digits(INT32_DIGITS_DEC))
# elif defined __COMPOUND_64__
#  define STATUS_LITERALISE_LENGTH(stat) \
     (utils_calc_digits(stat.value) + strlen(stat.description) + 2 + \
          utils_calc_digits(INT64_DIGITS_DEC))
# endif

# define STATUS_LITERALISE_FORMAT  "%d \"%s\" %d %p"

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

// # define REPORT_LITERALISE_HEADER_FORMAT_LENGTH(buff, PRIORITY, STATUSNAME,          \
//                                                ORIGINATOR, DESCRIPTION)        \
// {                                                                              \
//   const time_t now = time(NULL);                                               \
//   (void)strflen(buff, 28, DATETIME_FORMAT, localtime(&now));                   \
//   *length = strlen(buff);                                                      \
// }

// # define REPORT_LITERALISE_CHAINS_FORMAT_LENGTH(FILEPATH, LINE, FUNCNAME)      \
//   (strlen(FILEPATH) + utils_calc_digits(LINE) +                                \
//    strlen(FUNCNAME) + 10)  // Does not count '\0'
// # define REPORT_LITERALISE_CHAINS_EXCLAIM_FORMAT_LENGTH                        \
//   REPORT_LITERALISE_CHAINS_FORMAT_LENGTH

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

bool   Location_Equals(Location lc1, Location lc2);
Status Status_Literalise(Status *inst, char *buff);
bool   Status_Equals(Status stat1, Status stat2);
void   StatusUtils_Dump(Status *inst, Status *store, int idx);
bool   StatusUtils_HasPrev(Status *inst);
bool   StatusUtils_IsOkay(Status inst);
bool   StatusUtils_IsValid(Status inst);
bool   StatusUtils_IsRecursive(Status inst);
int    StatusUtils_Depth(Status *inst);

Status
Report_Create(Report *inst, Status *stat, FILE *dest, char *initiator,
              int priority);
bool
Report_Equals(Report repo1, Report repo2);
Status
Report_Literalise(Report *inst, char *buff);


Status
ReportSender_Create(ReportSender *inst, Report *report);
Status
ReportSender_Send(ReportSender *inst, ReportSendingTask task);
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

static Status UnknownStatus = {
  .description = "An unknown status.",
  .characteristic = STATUS_UNKNOWN,
  .prev = NULL
};

static Status NormalStatus = {
  .description = "A normal status.",
  .characteristic = STATUS_NORMAL,
  .prev = NULL
};

static Status ErrorStatus = {
  .description = "An error status.",
  .characteristic = STATUS_ERROR,
  .prev = NULL
};

// ----------------------EXTENDED--------------------------

static Status MemoryViolation = {
  .description = "Illegal access on certain memory address.",
  .characteristic = STATUS_ERROR,
  .prev = &ErrorStatus
};

static Status NullPointerAccounted = {
  .description = "An involving null pointer was not accepted.",
  .characteristic = STATUS_ERROR,
  .prev = &MemoryViolation
};

static Status InvalidObject = {
  .description = "An invalid object was presented.",
  .characteristic = STATUS_ERROR,
  .prev = &ErrorStatus
};

static Status UnavailableObject = {
  .description = "An unavailable object was presented.",
  .characteristic = STATUS_ERROR,
  .prev = &ErrorStatus
};

static Status InvalidParameter = {
  .description = "An invalid parameter was presented.",
  .characteristic = STATUS_ERROR,
  .prev = &InvalidObject
};

static Status InsufficientMemory = {
  .description = "Not enough room for further memory allocations.",
  .characteristic = STATUS_ERROR,
  .prev = &ErrorStatus
};

static Status ArithmeticError = {
  .description = "An arithmetic error occurred.",
  .characteristic = STATUS_ERROR,
  .prev = &ErrorStatus
};

static Status RuntimeError = {
  .description = "A runtime error occurred.",
  .characteristic = STATUS_ERROR,
  .prev = &ErrorStatus
};

static Status ArrayLengthError = {
  .description = "Given array length does not meet the requirement.",
  .characteristic = STATUS_ERROR,
  .prev = &ErrorStatus
};

static Status VariableFormatMismatch = {
  .description = "Given format does not match with given subjects.",
  .characteristic = STATUS_ERROR,
  .prev = &ErrorStatus
};

static Status ImprecisionError = {
  .description = "Precision was not enough for handling the calculation.",
  .characteristic = STATUS_ERROR,
  .prev = &RuntimeError
};

// ---------------------USER DEFINED-----------------------

static Status UnavailableInstance = {
  .description = "An unavailable instance was given for initialisation.",
  .characteristic = STATUS_ERROR,
  .prev = &NullPointerAccounted
};

static Status UnavailableParameter = {
  .description = "An unavailable instance was given as a parameter.",
  .characteristic = STATUS_ERROR,
  .prev = &UnavailableInstance
};

static Status InvalidReportTask = {
  .description = "An unavailable or illegal report task was given.",
  .characteristic = STATUS_ERROR,
  .prev = &InvalidParameter
};

static Status UnableToThrowError = {
  .description = "Unable to report an exceptional situation.",
  .characteristic = STATUS_ERROR,
  .prev = &RuntimeError
};

static Status ReadWriteError = {
  .description = "Error occurred during IO session.",
  .characteristic = STATUS_ERROR,
  .prev = &RuntimeError
};

static Status FileNotFound = {
  .description = "Target file was unavailable and unable to find.",
  .characteristic = STATUS_ERROR,
  .prev = &ReadWriteError
};

static Status InvalidFileName = {
  .description = "Given file name was invalid.",
  .characteristic = STATUS_ERROR,
  .prev = &ReadWriteError
};

static Status UnavailableFileName = {
  .description = "Given file name was unavailable",
  .characteristic = STATUS_ERROR,
  .prev = &UnavailableObject
};

static Status ReportThrown = {
  .description = "This function has thrown a report, "
                 "following instructions aborted.",
  .characteristic = STATUS_ERROR,
  .prev = &RuntimeError
};

static Status ReportMessageTooLong = {
  .description = "Given message is too long.",
  .characteristic = STATUS_ERROR,
  .prev = &ArrayLengthError
};

static Status MaximumLengthExceeded = {
  .description = "Buffer was too long.",
  .characteristic = STATUS_ERROR,
  .prev = &ArrayLengthError
};

static Status MaximumLiteralisationLengthExceeded = {
  .description = "Literalisation was too long.",
  .characteristic = STATUS_ERROR,
  .prev = &MaximumLengthExceeded
};

static Status UnavailableBuffer = {
  .description = "Given buffer was unavailable.",
  .characteristic = STATUS_ERROR,
  .prev = &UnavailableInstance
};

static Status InvalidLiteralisingBuffer = {
  .description = "Given buffer does not have a good integrity on its length.",
  .characteristic = STATUS_ERROR,
  .prev = &InvalidObject
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
