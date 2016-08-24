#include <assert.h>

#include "../src/process.h"

typedef struct {
  int count1;
  int count2;
  int *pid1;
  int *pid2;
  uint64_t *times1;
  uint64_t *times2;
  int interval;
} pop_data_t;

typedef struct {
  process_list_t *r1;
  process_list_t *r2;
} pop_res_t;

pop_res_t populate(pop_data_t data) {
  pop_res_t res;

  res.r1 = calloc(1, sizeof(process_list_t));
  res.r2 = calloc(1, sizeof(process_list_t));
  res.r1->processes = res.r2->processes = NULL;

  for (int i = 0; i < data.count1; i++) {
    process_t *to_add = calloc(1, sizeof(process_t));
    to_add->tgid = data.pid1[i];
    to_add->utime = data.times1[i];
    to_add->stime = 0;
    LL_APPEND(res.r1->processes, to_add);
  }
  for (int i = 0; i < data.count2; i++) {
    process_t *to_add = calloc(1, sizeof(process_t));
    to_add->tgid = data.pid2[i];
    to_add->utime = data.times2[i];
    to_add->stime = 0;
    LL_APPEND(res.r2->processes, to_add);
  }

  res.r1->timestamp = 50000;
  res.r2->timestamp = 50000 + data.interval;
  LL_SORT(res.r1->processes, pidcmp);
  LL_SORT(res.r2->processes, pidcmp);

  return res;
}

void check(process_usage_t *usages, int *r_pid, unsigned int *r_deltas) {
  process_usage_t *elt = NULL;
  int i = 0;
  DL_FOREACH(usages, elt) {
    assert(elt->tgid == r_pid[i]);
    assert(elt->time_in_measure == r_deltas[i]);
    i++;
  }
}

void simple_test() {
  int r_pid[] = {1, 3, 5, 6, 8, 11, 12};
  unsigned int r_deltas[] = {0, 1, 3, 1, 0, 2, 0};

  int pid[] = {1, 5, 6, 3, 8, 12, 11};
  uint64_t times1[] = {1, 1, 2, 0, 0, 0, 5};
  uint64_t times2[] = {1, 4, 3, 1, 0, 0, 7};

  pop_data_t t1 = {.count1 = 7,
                   .count2 = 7,
                   .pid1 = pid,
                   .pid2 = pid,
                   .times1 = times1,
                   .times2 = times2,
                   .interval = 50};
  pop_res_t r = populate(t1);

  process_usage_list_t *n = process_get_usage(r.r1, r.r2);

  check(n->usages, r_pid, r_deltas);
}

int main() {
  simple_test();
  return 0;
}