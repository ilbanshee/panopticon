#ifndef PROCESS_H_
#define PROCESS_H_
#include <sys/types.h>

typedef struct {
  // some config?

  // processes
  // users of running processes

  uid_t current_id;

  int total_tasks;
  int running_tasks;
  int userland_threads;
  int kernel_threads;

  unsigned long long int totalMem;
  unsigned long long int usedMem;
  unsigned long long int freeMem;
  unsigned long long int sharedMem;
  unsigned long long int buffersMem;
  unsigned long long int cachedMem;
  unsigned long long int totalSwap;
  unsigned long long int usedSwap;
  unsigned long long int freeSwap;

  int cpu_count;
} process_list;

typedef struct {

} process;

int test();

process_list *process_list_new(uid_t user_id);
void process_list_delete(process_list *pl);

void process_list_add(process_list *this, process *p);

void process_list_remove(process_list *this, process *p);

process *process_list_get(process_list *this, int idx);

int process_list_size(process_list *this);

void process_list_sort(process_list *this);

#endif // PROCESS_H_
