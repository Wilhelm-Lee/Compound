#ifndef COMPOUND_STATUS_H
# define COMPOUND_STATUS_H

# include <stdbool.h>
# include <stdint.h>
# include <stdlib.h>
# include <string.h>
# include <threads.h>
# include <time.h>
# include <stdio.h>

# define __HERE__  { .file = __FILE__, .line = __LINE__, .func = __func__ }

/* Status characteristics */
typedef enum {
  STATUS_UNKNOWN  = -1,
  STATUS_NORMAL   = 0,
  STATUS_ERROR    = 1
} StatusCharacteristics;

//!!! UNKNOWN ENTITY !!!
// enum {
//   ARGUE_FINALISED = 0b01,
//   ARGUE_UNSETTLED =-0b01,

//   ARGUE_POSITIVE =  0b10,
//   ARGUE_NEGATIVE = -0b10,
// };

/* Indicated the exact location where the "issue" was occurred at. */
typedef struct {
  const char *file;
  int line;
  const char *func;
} Location;

/* Common return type for reporting functions that require to give more
   information about the procedure. */
typedef struct _Status {
  int value;  /* Traditional returning data "int".  Only used when the function
                 called and received legacy functions that uses "int" as the
                 returning type that wish to have place to hold the value.
                 Otherwise, the function would just return the structure Status. */
  char *description;
  int characteristic;
  struct _Status *prev;
} Status;

/* "Report" recollects essential informations, included but not limited to
   Status and others for making an report for debugging and such. */
typedef struct {
  Status stat;
  Location pos;
  char *originator;
  struct tm time;
  int priority;
} Report;

typedef struct {
  FILE **dests_ref;
  Report **reports_ref;
  int totalreports;
  int reportsent;
  struct timespec elapsed;
} ReportSender;

typedef Status (*ReportSendingTask) (FILE **, Report *);
typedef int ReportSendingTaskID;

typedef struct {
  ReportSendingTask *tasks_ref;
  int sendercount;
  int finishedcount;
  Status *senderreturn;
} ReportSendingManager;

typedef enum {
  REPORT_SENDING_TASK_STATUS_FINISHED = 0,
  REPORT_SENDING_TASK_STATUS_PENDING,
  REPORT_SENDING_TASK_STATUS_PROCEEDING,
  REPORT_SENDING_TASK_STATUS_PAUSED,
  REPORT_SENDING_TASK_STATUS_NOTFOUND
} ReportSendingTaskStatus;

typedef Status (*Argue) (Report);

typedef struct {
  Argue argue_start;  // Implicitly converting thrd_start_t to Argue
  void *external_param;
} ArgueStarter;

/* Argument levels (qualities) */
typedef enum {
  ARGUMENT_NONE = 0,
  ARGUMENT_MINOR,
  ARGUMENT_NORMAL,
  ARGUMENT_MAJOR,
  ARGUMENT_CRITICAL,
  ARGUMENT_SEVERE,
  ARGUMENT_ALL,
} ArgumentLevel;

typedef struct {
  ReportSender *streams;
  ArgueStarter handler;
  ArgumentLevel level;
  bool muted;
} Argument;

// typedef struct {
//   Argument *members;
//   int (*announcer) (Argument);
// } ArgumentAnnouncer;

# define STATUS_BUFFER_MAXIMUM_LENGTH  UINT32_MAX


bool
status_issueposition_compare(Location lc1, Location lc2);
bool
status_hasprev(Status stat);
bool
status_isnormal(Status stat);
bool
status_isvalid(Status stat);
bool
status_recursive(Status stat);
void
status_dump(Status stat, Status *statbuff, int idx);
bool
status_compare(Status stat1, Status stat2);


bool
report_compare(Report repo1, Report repo2);


Status
reportsender_create(ReportSender *inst, FILE **dests_ref,
                    ReportSendingTask *tasks_ref);
Status
reportsender_send(ReportSender sender, ReportSendingTask *tasks_ref);


ReportSendingTaskID
reportsendingmanager_appendtask(ReportSendingManager *inst,
                                ReportSendingTask task);
Status
reportsendingmanager_removetask(ReportSendingManager *inst,
                                ReportSendingTaskID taskid);
ReportSendingTaskStatus
reportsendingmanager_taskstatus(ReportSendingManager *inst,
                                ReportSendingTaskID taskid);


Status
arguestarter_create(ArgueStarter *inst, void *external_param);
Status
arguestarter_constr(ArgueStarter *inst, Argue argue_start,
                    void *external_param);
Status
arguestarter_start(ArgueStarter *inst);
static inline int
arguestarter_equal(ArgueStarter *inst1, ArgueStarter *inst2)
{
  return (inst1->argue_start == inst2->argue_start)
          || (inst1->external_param == inst2->external_param);
}
Status arguestarter_current(void);
Status arguestarter_sleep(const struct timespec *time_point,
                          struct timespec *remaining);
void arguestarter_exit(int code) __attribute__ ((__noreturn__));
Status arguestarter_join(Argue thrd);


Status
argument_create(Argument *inst, ReportSender *streams, ArgueStarter handler);
Status
argument_constr(Argument *inst, ReportSender *streams, ArgueStarter handler,
                int level, bool muted);


// --------------------------------------------------------

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

// --------------------------------------------------------

static Status NullPointerAccounted = {
  .description = "An involving null pointer was not accepted.",
  .characteristic = STATUS_ERROR,
  .prev = &ErrorStatus
};

static Status InvalidParameter = {
  .description = "An invalid parameter was presented.",
  .characteristic = STATUS_ERROR,
  .prev = &ErrorStatus
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

#endif  /* COMPOUND_STATUS_H */
