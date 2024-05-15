#include <Compound/common.h>
#include <Compound/status.h>

bool status_issuelocation_equal(Location lc1, Location lc2) {
  return ((!strcmp(lc1.file, lc2.file)) && (lc1.line == lc2.line) &&
          (!strcmp(lc1.func, lc2.func)));
}

bool status_hasprev(Status stat) {
  /* Skip when stat is unavailable for accessing. */
  state(status_equal(stat, (Status){}), false);

  return (stat.prev != NULL);
}

bool status_isokay(Status stat) { return (!stat.characteristic); }

bool status_isvalid(Status stat) {
  return (!strcmp(stat.description, "") && stat.characteristic >= 0 &&
          stat.prev != NULL);
}

bool status_recursive(Status stat) {
  return (stat.prev != NULL && stat.prev == &stat);
}

void status_dump(Status inst, Status *statbuff, int idx) {

  /* Skip when either stat or stat.prev is unavailable, or, idx is invalid. */
  solve((statbuff == NULL || !status_hasprev(inst) || idx < 0), return;);

  statbuff[idx] = inst;
  (void)printf("status_dump: Index %d has assigned with %p\n", idx, &inst);

  status_dump(*inst.prev, statbuff, ++idx);
}

bool status_equal(Status stat1, Status stat2) {

  /* Skip when both stat1 and stat2 are empty. */
  state((stat1.value == 0 && stat2.value == 0 && stat1.description == 0x0 &&
         stat2.description == 0x0 && stat1.characteristic == 0 &&
         stat2.characteristic == 0 && stat1.prev == 0x0 && stat2.prev == 0x0),
        true);

  /* True for equality; False for inequality. */
  return ((stat1.value == stat2.value) &&
          (!strcmp(stat1.description, stat2.description)) &&
          (stat1.characteristic == stat2.characteristic) &&
          (status_equal(*stat1.prev, *stat2.prev)));
}

int status_depth(Status *stat) {
  /* Skip unavailable stat. */
  state((stat == NULL), -1);
  
  Status *p = stat;  // Include this layer of Status.
  int cnt = 1;
  while (p != NULL) {
    if (status_recursive(*p) || !status_hasprev(*stat))  break;
    
    p = p->prev;
    cnt += 1;
  }
  
  return cnt;
}

Status report_create(Report *inst, Status *stat, FILE *dest, char *originator,
                     int priority) {
  /* Skip unavailable parameters. */
  fails(inst, UnavailableInstance);
  fails(stat, error(InvalidParameter, "Given originator was null."));
  fails(originator, error(InvalidParameter, "Given originator was null."));
  state(priority < 0, error(InvalidParameter, "Given priority was negative."));

  /* Copy and assign. */
  inst->stat = *stat;
  inst->originator = originator;
  inst->time = time(NULL);
  inst->priority = priority;
  inst->status = REPORT_SENDING_TASK_STATUS_PENDING;
  inst->dest = (dest == NULL ? stderr : dest);

  return NormalStatus;
}

Status report_literalise(Report *inst, char *buff) {
  /* Skip when inst or buff is unavailable. */
  fails(inst, UnavailableInstance);
  fails(buff, error(InvalidParameter, "Given buffer was unavailable."));

  const int depth = status_depth(&inst->stat);
  char buff // TODO(william): HERE
}

Status reportsender_create(ReportSender *inst, Report *report) {
  fails(inst, UnavailableInstance);
  fails(report, error(UnavailableParameter, "Given report was unavailable."));

  thrd_create(&inst->thread, &HANDLER, report);
  *inst->report = *report;
  inst->elapsed = 0;
  inst->result = REPORT_SENDER_RESULT_PENDING;
  inst->successful = false;

  return NormalStatus;
}

Status reportsender_send(ReportSender *inst, ReportSendingTask task) {
  /* Skip when inst or task is unavailable. */
  fails(inst,
        error(UnavailableInstance, "Report sender was given unavailable."));
  fails(task, InvalidReportTask);

  /* Assign for dest. */
  const FILE *dest = (inst->report->dest == NULL ? stdout : inst->report->dest);
  // char buff[];
  // TODO(william): HERE, report_literalise

  /* Write/Send data. */
  inst->report->status = REPORT_SENDING_TASK_STATUS_PROCEEDING;
  if (!fprintf(dest, buff)) {
  }
   
  /* Sent successfully!  Mark down properties. */
}

// bool arguestarter_equal(ArgueStarter *inst1, ArgueStarter *inst2)
// {
//   /* Skip when either inst1 or inst2 is unavailable. */
//   state(inst1 == NULL || inst2 == NULL, false);

//   return (inst1->argue_start == inst2->argue_start)
//           || (inst1->external_param == inst2->external_param);
// }

ReportSendingTaskID _throw(Report report, Location loc) {
  // /* Create new a instance of ReportSender. */
  // ReportSender sender;
  // reportsender_create(&sender, stderr);

  // /* Send message. */
  // /* Initialise sender's thread. */
  // thrd_t sending;
  // /* Skip on failing on creating thread. */
  // if (!thrd_create(&sending, starter, NULL)) {

  //   /* Conclude the session of sender. */
  //   report.status = REPORT_SENDING_TASK_STATUS_FINISHED,
  //   report.result = (ARGUE_RESULT_FINALISED | ARGUE_RESULT_NEGATIVE);

  //   sender.result = REPORT_SENDER_RESULT_FINISHED;
  //   sender.successful = false;

  //   return -1;
  // }

  // /* Perform sending. */
  // reportsender_send(&sender, NULL);

  /* Initialise sender. */
  ReportSender sender;
  /* Return with -1 when initialisation failed. */
  state(!(status_isokay(reportsender_create(&sender, &report))), -1);
  
  /* Inject location information.  Could be more elegant, though. */
  sender.report->stat.loc = loc;
  
  /* Send. */ /* Return -1 when failed on sending. */
  state(!status_isokay(reportsender_send(&sender, HANDLER)), -1);
  
  
}
