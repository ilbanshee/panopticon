#include <assert.h>

#include "../src/process.h"

int main() {
  int count = 7;
  int pid[] = {1, 5, 6, 3, 8, 12, 11};
  uint64_t times1[] = {1, 1, 2, 0, 0, 0, 5};
  uint64_t times2[] = {1, 4, 3, 1, 0, 0, 7};

  int r_pid[] = {1, 3, 5, 6, 8, 11, 12};
  unsigned int r_deltas[] = {0, 1, 3, 1, 0, 2, 0};

  process_list_t *r1, *r2;
  r1 = calloc(1, sizeof(process_list_t));
  r2 = calloc(1, sizeof(process_list_t));
  r1->processes = r2->processes = NULL;

  for (int i = 0; i < count; i++) {
    process_t *to_add = calloc(1, sizeof(process_t));
    to_add->tgid = pid[i];
    to_add->utime = times1[i];
    to_add->stime = 0;
    LL_APPEND(r1->processes, to_add);

    to_add = calloc(1, sizeof(process_t));
    to_add->tgid = pid[i];
    to_add->utime = times2[i];
    to_add->stime = 0;
    LL_APPEND(r2->processes, to_add);
  }

  r1->timestamp = 50000;
  r2->timestamp = 50010;
  LL_SORT(r1->processes, pidcmp);
  LL_SORT(r2->processes, pidcmp);

  process_usage_list_t *n = process_get_usage(r1, r2);

  process_usage_t *elt = NULL;
  int i = 0;
  DL_FOREACH(n->usages, elt) {
    assert(elt->tgid == r_pid[i]);
    assert(elt->time_in_measure == r_deltas[i]);
    i++;
  }
  return 0;
}
