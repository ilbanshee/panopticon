/* -*- mode: c; c-file-style: "k&r"; c-indent-level 2 -*- */
/*
 * Copyright (c) 2016 Marco
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef PROCESS_H_
#define PROCESS_H_
#include <sys/types.h>
#include <uthash.h>
#include <utlist.h>

typedef enum {
  STATE_UNKNOWN,
  STATE_NORMAL,
  STATE_NEW,
  STATE_DELETED
} process_state;

typedef struct process_s {
  int tgid; /* task group ID, the POSIX PID */
  int tid;  /* task id, the POSIX thread ID */
  int ppid; /* pid of parent process */

  char *name;             /* executable name */
  char *cmdline;          /* full path command line */
  int user;               /* real user id for process */
  int32_t threads_num;    /* number of threads */
  uint64_t vms;           /* virtual memory size in Kb */
  uint64_t rss;           /* resident set size in Kb */
  uint64_t utime;         /* total user time in minutes.seconds * TIME_TO_MIN */
  uint64_t stime;         /* total system time for process in minutes.seconds *
                             TIME_TO_MIN */
  struct process_s *next; /* pointer to the next structure in list */
} process_t;

typedef struct process_usage_s {
  int tgid;
  process_state state;
  uint64_t time_in_measure;
  uint32_t percent_in_measure;
  struct process_usage_s *next;
} process_usage_t;

typedef struct user_s {
  int user;
  char *name;
  struct user_s *next;
} user_t;

typedef struct process_usage_list_s {
  process_usage_t *usages;
  uint64_t timestamp;
  uint64_t measure_delta;
} process_usage_list_t;

typedef struct process_list_s {
  // some config?

  process_t *processes;
  user_t *users;

  uid_t current_id;

  int total_tasks;
  int running_tasks;
  int userland_threads;
  int kernel_threads;

  uint64_t totalMem;
  uint64_t usedMem;
  uint64_t freeMem;
  uint64_t sharedMem;
  uint64_t buffersMem;
  uint64_t cachedMem;
  uint64_t totalSwap;
  uint64_t usedSwap;
  uint64_t freeSwap;

  int cpu_count;
  uint64_t timestamp; /* in microseconds */
} process_list_t;

/** Merge two processes list to get cpu time usage stats.
 *
 *  this function will merge two process_list_t and return a
 *  process_usage_list_t struct with a sorted list
 *  of process_usage_t structs inside.
 *
 *  @warning this function will sort input list in-place so
 *  a_list and b_list will contain the same elements after call
 *  returns but sorted in ascending pid order.
 */
process_usage_list_t *process_get_usage(process_list_t *a_list,
                                        process_list_t *b_list);

process_list_t *process_list_new(uid_t user_id);
void process_list_delete(process_list_t *pl);

void process_list_add(process_list_t *this, process_t *p);
void process_list_remove(process_list_t *this, process_t *p);

process_t *process_list_get(process_list_t *this, int idx);

int process_list_size(process_list_t *this);

void process_list_sort(process_list_t *this);

int process_list_processes(process_list_t **result);

void process_usage_print(process_usage_t *process);
void process_print(process_t *process);
#endif // PROCESS_H_
