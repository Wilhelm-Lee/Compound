#include <Compound/status.h>

Status Location_Literalise(Location *inst, char *buff)
{
  fails(inst, apply(UnavailableInstance));
  fails(buff, apply(UnavailableBuffer));
  
  /* Literalise line. */
  char line_buff[LITERALISATION_LENGTH_MAXIMUM];
  Utils_LiteraliseInteger(inst->line, line_buff);
  
  /* Concatenate every buff. */
  const long total_len = strlen(strnil(inst->file)) + strlen(strnil(line_buff))
                           + strlen(strnil(inst->func))
                           + LOCATION_LITERALISE_FORMAT_LENGTH;

  state(total_len > LITERALISATION_LENGTH_MAXIMUM,
    apply(MaximumLiteralisationLengthExceeded));
  
  /* Copy and assign. */
  // return value(UnknownStatus,
  //              !sprintf(buff, LOCATION_LITERALISE_FORMAT,
  //                       inst->file, inst->line, inst->func));
  const int written =
    sprintf(buff, LOCATION_LITERALISE_FORMAT,inst->file,inst->line,inst->func);

  // written in "value" is absolutely ZERO.
  state(!written, apply(value(UnknownStatus, written)));
  
  return apply(NormalStatus);
}

bool Location_Equal(Location *lc1, Location *lc2)
{
  state(lc1 == NULL || lc2 == NULL, false);

  return ((!strcmp(lc1->file, lc2->file)) && (lc1->line == lc2->line) &&
          (!strcmp(lc1->func, lc2->func)));
}

bool Status_Equal(Status *stat1, Status *stat2)
{
  state(stat1 == NULL || stat2 == NULL, false);

  return (
    stat1->value == stat2->value &&
    !strcmp(stat1->description, stat2->description) &&
    stat1->characteristic == stat2->characteristic &&
    Location_Equal(&stat1->loc, &stat2->loc) &&
    ((StatusUtils_HasPrev(*stat1) && StatusUtils_HasPrev(*stat2))
            ? Status_Equal(stat1->prev, stat2->prev)
            : true)
  );
}

Status Status_Literalise(Status *inst, char *buff)
{
  /* Skip unavailable instance and invalid parameter. */
  fails(inst, apply(UnavailableInstance));
  fails(buff, apply(UnavailableBuffer));

  /* Literalise loc. */
  char loc_buff[LITERALISATION_LENGTH_MAXIMUM];
  notok(Location_Literalise(&inst->loc, loc_buff), {
    return _;
  });

  /* Styling output. */
  const char *fmt;
  if (inst->characteristic == STATUS_ERROR) {
    // TODO(william): Replace following line with coloured-term-output function.
    fmt = "\e[38;5;9m\e[4m\e[1m"STATUS_LITERALISE_FORMAT"\e[0m";
  } else if (inst->characteristic == STATUS_UNKNOWN) {
    // TODO(william): Replace following line with coloured-term-output function.
    fmt = "\e[38;5;11m\e[4m"STATUS_LITERALISE_FORMAT"\e[0m";
  } else {
    // TODO(william): Replace following line with coloured-term-output function.
    fmt = "\e[38;5;10m\e[2m"STATUS_LITERALISE_FORMAT"\e[0m";
  }

  /* Concatenate every buffer. */
  state(!sprintf(buff, fmt,
                 inst->identity, inst->description,
                 (!inst->prev ? "(null)" : (inst->prev->identity)),
                 inst->value, inst->characteristic, loc_buff),
    apply(error(ReadWriteError, "No bytes were written into buffer.")));

  return apply(NormalStatus);
}

bool StatusUtils_HasPrev(Status stat)
{
  return (stat.prev != NULL);
}

bool StatusUtils_IsOkay(Status stat)
{
  return (!stat.characteristic);
}

bool StatusUtils_IsValid(Status stat)
{
  return (!strcmp(stat.description, "") && stat.characteristic >= 0
          && !stat.prev);
}

bool StatusUtils_IsRecursive(Status stat)
{
  return (stat.prev && stat.prev == &stat);
}

void StatusUtils_Dump(Status *inst, Status *store, int idx)
{

  /* Skip when either stat or stat.prev is unavailable, or, idx is invalid. */
  svoid((!store || !StatusUtils_HasPrev(*inst) || idx < 0));

  store[idx] = *inst;
  
  StatusUtils_Dump(inst->prev, store, ++idx);
}

int StatusUtils_Depth(Status *stat)
{
  /* Skip unavailable stat. */
  state((!stat || !stat->prev), -1);
  
  Status *p = stat;  // Include this layer of Status.
  register int cnt;
  for (cnt = 0; (!StatusUtils_IsRecursive(*p)
       && StatusUtils_HasPrev(*p)); cnt++)  p = p->prev;

  return cnt;
}

Status Report_Create(Report *inst, Status *stat, FILE *dest, char *initiator,
                     int priority)
{
  /* Skip unavailable parameters. */
  fails(inst, apply(UnavailableInstance));
  fails(stat, apply(error(InvalidParameter, "Given stat was null.")));
  fails(initiator, apply(error(InvalidParameter, "Given initiator was null.")));
  state(priority < 0, apply(error(InvalidParameter, "Given priority was negative.")));

  /* Copy and assign. */
  inst->status = *stat;
  inst->initiator = calloc(strlen(initiator), sizeof(char));
  (void)strcpy(inst->initiator, initiator);
  inst->time = time(NULL);
  inst->priority = priority;
  inst->taskprint_status = REPORT_SENDING_TASK_STATUS_PENDING;
  inst->dest = (dest == NULL ? stdout : dest);

  return apply(NormalStatus);
}

Status Report_CopyOf(Report *inst, Report *other)
{
  fails(inst, apply(UnavailableInstance));
  fails(other, apply(error(InvalidParameter, "Given report is unavailable.")));

  // Status status;
  // char *initiator;
  // time_t time;
  // ReportSendingPriority priority;
  // ReportSendingTaskStatus taskprint_status;
  // FILE *dest;
  inst->status = other->status;

  return apply(NormalStatus);
}

void Report_Delete(Report *inst)
{
  svoid(inst);

  free(inst->initiator);
  inst->initiator = NULL;
  inst->dest = NULL;
  inst->priority = 0;
  inst->status = (Status){};
  inst->taskprint_status = REPORT_SENDING_TASK_STATUS_NOTFOUND;
  inst->time = 0;
  inst = NULL;
}

Status Report_Literalise(Report *inst, char *buff)
{
  fails(inst, apply(UnavailableInstance));
  fails(buff, apply(UnavailableBuffer));
  // state(strlen(buff) != LITERALISATION_LENGTH_MAXIMUM,
  //       InvalidLiteralisingBuffer);
  
  /* Report literalisation. */
  int idx = 0;
  char report_literalising[LITERALISATION_LENGTH_MAXIMUM];
  
  /** Status literalisation. **/
  char status_literalising[LITERALISATION_LENGTH_MAXIMUM];
  (void)Status_Literalise(&inst->status, status_literalising);
  idx += strlen(status_literalising);
  /** fin **/
  
  /** Initiator literalisation. **/
  idx += strlen(inst->initiator);
  /** fin **/
  
  /** Time literalisation. **/
  char time_literalising[LITERALISATION_LENGTH_MAXIMUM];
  idx += Utils_LiteraliseInteger(inst->time, time_literalising);
  /** fin **/
  
  /** Priority literalisation. **/
  char priority_literalising[LITERALISATION_LENGTH_MAXIMUM];
  idx += Utils_LiteraliseInteger(inst->priority, priority_literalising);
  /** fin **/
  
  /** Taskprint_status literalisation. **/
  char taskprint_status_literalising[LITERALISATION_LENGTH_MAXIMUM];
  idx += Utils_LiteraliseInteger(inst->taskprint_status, taskprint_status_literalising);
  /** fin **/
  
  /** Dest literalisation. **/
  char dest_literalising[LITERALISATION_LENGTH_MAXIMUM];
  idx += Utils_LiteraliseInteger((long long int)inst->dest, dest_literalising);
  /** fin **/
  
  if (idx >= LITERALISATION_LENGTH_MAXIMUM) {
    buff = NULL;
    return MaximumLiteralisationLengthExceeded;
  }

  strcpy(report_literalising, status_literalising);
  strcpy(report_literalising, time_literalising);
  strcpy(report_literalising, priority_literalising);
  strcpy(report_literalising, taskprint_status_literalising);
  strcpy(report_literalising, dest_literalising);
  
  strcpy(buff, report_literalising);
  /* fin */
  
  return apply(NormalStatus);
}

Status ReportSender_Create(ReportSender *inst, Report *report)
{
  fails(inst, apply(UnavailableInstance));
  fails(report, error(UnavailableParameter, "Given report was unavailable."));

  thrd_create(&inst->thread, &HANDLER, report);
  notok(Report_CopyOf(inst->report, report),
    return error(ErrorStatus, "Cannot copy to create new instance of report.");
  )  // *inst->report = *report;
  inst->elapsed = 0;
  inst->result = REPORT_SENDER_RESULT_PENDING;
  inst->successful = false;

  return apply(NormalStatus);
}

Status ReportSender_Send(ReportSender *inst, ReportSendingTask task)
{
  // /* Skip when inst or task is unavailable. */
  // fails(inst,
  //       error(UnavailableInstance, "Report sender was given unavailable."));
  // fails(task, InvalidReportTask);

  // /* Assign for dest. */
  // const FILE *dest = (inst->report->dest == NULL ? stdout : inst->report->dest);
  // // char buff[];
  // // TODO(william): HERE, Report_Literalise

  // /* Write/Send data. */
  // inst->report->taskprint_status = REPORT_SENDING_TASK_STATUS_PROCEEDING;
  // if (!fprintf(dest, buff)) {
  // }
   
  // /* Sent successfully!  Mark down properties. */
  return apply(NormalStatus);
}

// bool arguestarter_equal(ArgueStarter *inst1, ArgueStarter *inst2)
// {
//   /* Skip when either inst1 or inst2 is unavailable. */
//   state(inst1 == NULL || inst2 == NULL, false);

//   return (inst1->argue_start == inst2->argue_start)
//           || (inst1->external_param == inst2->external_param);
// }

ReportSendingTaskID _throw(Report report, Location loc)
{
  // // /* Create new a instance of ReportSender. */
  // // ReportSender sender;
  // // ReportSender_Create(&sender, stderr);

  // // /* Send message. */
  // // /* Initialise sender's thread. */
  // // thrd_t sending;
  // // /* Skip on failing on creating thread. */
  // // if (!thrd_create(&sending, starter, NULL)) {

  // //   /* Conclude the session of sender. */
  // //   report.status = REPORT_SENDING_TASK_STATUS_FINISHED,
  // //   report.result = (ARGUE_RESULT_FINALISED | ARGUE_RESULT_NEGATIVE);

  // //   sender.result = REPORT_SENDER_RESULT_FINISHED;
  // //   sender.successful = false;

  // //   return -1;
  // // }

  // // /* Perform sending. */
  // // ReportSender_Send(&sender, NULL);

  // /* Initialise sender. */
  // ReportSender sender;
  // /* Return with -1 when initialisation failed. */
  // state(!(StatusUtils_IsOkay(ReportSender_Create(&sender, &report))), -1);
  
  // /* Inject location information.  Could be more elegant, though. */
  // sender.report->status.loc = loc;
  
  // /* Send. */ /* Return -1 when failed on sending. */
  // state(!StatusUtils_IsOkay(ReportSender_Send(&sender, HANDLER)), -1);
  
  return 0;
}

int HANDLER(void *report)
{
  // /* Throw UnableToThrowError when param is unavailable. */
  // if (report == NULL) {
  //   /* Create report on this. */
  //   Report e;
  //   Report_Create(
  //     &e,
  //     &error(UnableToThrowError, "Cannot perform throwing.  Aborted."),
  //     stderr, nameof(DEFAULT_ARGUE_STARTER),
  //     REPORT_SENDING_PRIORITY_FATAL);

  //   /* Perform throwing. */
  //   (void)throw(e); // Throw the report alone.
  //   return 1;
  // }

  // (void)throw(*(Report *)report); // Lonely throw, no catch will company.
  // return 0;
  
  return 0;
}
