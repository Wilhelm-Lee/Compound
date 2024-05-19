#include <Compound/common.h>
#include <Compound/status.h>
#include <Compound/utils.h>

bool Location_Equal(Location lc1, Location lc2)
{
  return ((!strcmp(lc1.file, lc2.file)) && (lc1.line == lc2.line) &&
          (!strcmp(lc1.func, lc2.func)));
}

bool Status_Equals(Status stat1, Status stat2)
{

  /* Skip when both stat1 and stat2 are empty. */
  state((stat1.value == 0 && stat2.value == 0 && stat1.description == 0x0 &&
         stat2.description == 0x0 && stat1.characteristic == 0 &&
         stat2.characteristic == 0 && stat1.prev == 0x0 && stat2.prev == 0x0),
        true);

  /* True for equality; False for inequality. */
  return ((stat1.value == stat2.value) &&
          (!strcmp(stat1.description, stat2.description)) &&
          (stat1.characteristic == stat2.characteristic) &&
          (Status_Equals(*stat1.prev, *stat2.prev)));
}

Status Status_Literalise(Status *inst, char *buff)
{
  /* Skip unavailable or invalid parameters. */
  fails(inst, UnavailableInstance);
  fails(buff, UnavailableBuffer);
  // state(strlen(buff) != LITERALISATION_LENGTH_MAXIMUM,
  //       InvalidLiteralisingBuffer);
  
  /* Set up idx for counting final literalisation length to ensure the
     string copying is index access safe. */
  int idx = 0;
  const int status_dump_buffer_len = StatusUtils_Depth(inst);
  
  Status status_dump_buffer[status_dump_buffer_len];

  /* Literalise every status that flattened on status_dump_buffer. */
  for (register int i = 0; i < status_dump_buffer_len; i++) {
    char status_literalising_buffer[LITERALISATION_LENGTH_MAXIMUM];
    (void)Status_Literalise(&status_dump_buffer[i], status_literalising_buffer);
    
    /* Append to buff. */
    /* Prevent buffer-out-of-bound access. */
    const int status_literalising_buffer_len = strlen(status_literalising_buffer);
    if (idx + status_literalising_buffer_len >= LITERALISATION_LENGTH_MAXIMUM) {
      buff = NULL;
      return MaximumLiteralisationLengthExceeded;
    }
    
    idx += status_literalising_buffer_len;
    (void)strcat(buff, status_literalising_buffer);
  }  
  
  return NormalStatus;
}

bool StatusUtils_HasPrev(Status *stat)
{
  /* Skip when stat is unavailable for accessing. */
  state(Status_Equals(*stat, (Status){}), false);

  return (stat->prev != NULL);
}

bool StatusUtils_IsOkay(Status stat)
{
  return (!stat.characteristic);
}

bool StatusUtils_IsValid(Status stat)
{
  return (!strcmp(stat.description, "") && stat.characteristic >= 0 &&
          stat.prev != NULL);
}

bool StatusUtils_IsRecursive(Status stat)
{
  return (stat.prev != NULL && stat.prev == &stat);
}

void StatusUtils_Dump(Status *inst, Status *store, int idx)
{

  /* Skip when either stat or stat.prev is unavailable, or, idx is invalid. */
  solve((store == NULL || !StatusUtils_HasPrev(inst) || idx < 0), return;);

  store[idx] = *inst;
  
  StatusUtils_Dump(inst->prev, store, ++idx);
}

int StatusUtils_Depth(Status *stat)
{
  /* Skip unavailable stat. */
  state((stat == NULL), -1);
  
  Status *p = stat;  // Include this layer of Status.
  int cnt = 1;
  while (p != NULL) {
    if (StatusUtils_IsRecursive(*p) || !StatusUtils_HasPrev(stat))  break;
    
    p = p->prev;
    cnt += 1;
  }
  
  return cnt;
}

Status Report_Create(Report *inst, Status *stat, FILE *dest, char *initiator,
                     int priority)
{
  /* Skip unavailable parameters. */
  fails(inst, UnavailableInstance);
  fails(stat, error(InvalidParameter, "Given initiator was null."));
  fails(initiator, error(InvalidParameter, "Given initiator was null."));
  state(priority < 0, error(InvalidParameter, "Given priority was negative."));

  /* Copy and assign. */
  inst->status = *stat;
  inst->initiator = initiator;
  inst->time = time(NULL);
  inst->priority = priority;
  inst->task_status = REPORT_SENDING_TASK_STATUS_PENDING;
  inst->dest = (dest == NULL ? stderr : dest);

  return NormalStatus;
}

Status Report_Literalise(Report *inst, char *buff)
{
  fails(inst, UnavailableInstance);
  fails(buff, UnavailableBuffer);
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
  
  /** Task_status literalisation. **/
  char task_status_literalising[LITERALISATION_LENGTH_MAXIMUM];
  idx += Utils_LiteraliseInteger(inst->task_status, task_status_literalising);
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
  strcpy(report_literalising, task_status_literalising);
  strcpy(report_literalising, dest_literalising);
  
  strcpy(buff, report_literalising);
  /* fin */
  
  return NormalStatus;
}

Status ReportSender_Create(ReportSender *inst, Report *report)
{
  fails(inst, UnavailableInstance);
  fails(report, error(UnavailableParameter, "Given report was unavailable."));

  thrd_create(&inst->thread, &HANDLER, report);
  *inst->report = *report;
  inst->elapsed = 0;
  inst->result = REPORT_SENDER_RESULT_PENDING;
  inst->successful = false;

  return NormalStatus;
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
  // inst->report->task_status = REPORT_SENDING_TASK_STATUS_PROCEEDING;
  // if (!fprintf(dest, buff)) {
  // }
   
  // /* Sent successfully!  Mark down properties. */
  return NormalStatus;
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

