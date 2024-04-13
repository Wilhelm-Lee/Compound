#include <Compound/status.h>

bool status_issueposition_compare(Location lc1, Location lc2) {
  return ((!strcmp(lc1.file, lc2.file)) && (lc1.line == lc2.line) &&
          (!strcmp(lc1.func, lc2.func)));
}

bool status_hasprev(Status stat) { return (stat.prev != NULL); }

bool status_isnormal(Status stat) { return (!stat.characteristic); }

bool status_recursive(Status stat) {
  return (stat.prev != NULL && stat.prev == &stat);
}

void status_dump(Status stat, Status *statbuff, int idx) {
  if (statbuff == NULL || !status_hasprev(stat) || idx < 0) {
    return;
  }

  statbuff[idx] = stat;

  status_dump(*stat.prev, statbuff, ++idx);
}

bool status_compare(Status stat1, Status stat2) {
  return (!strcmp(stat1.description, stat2.description) &&
          (stat1.characteristic == stat2.characteristic) &&
          (!status_compare(*stat1.prev, *stat2.prev)));
}
