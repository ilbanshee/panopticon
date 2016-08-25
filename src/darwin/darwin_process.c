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
#include "darwin_process.h"
#include <errno.h>
#include <inttypes.h>
#include <libproc.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>

#include <mach/mach.h>
#include <mach/mach_init.h>
#include <mach/thread_policy.h>

#include "../stats.h"

/* see https://developer.apple.com/legacy/library/qa/qa2001/qa1123.html */
/*
 * Returns a list of all BSD processes on the system.  This routine
 * allocates the list and puts it in *procList and a count of the
 * number of entries in *procCount. You are responsible for freeing
 * this list (use "free" from System framework).
 * On success, the function returns 0.
 * On error, the function returns a BSD errno value.
 */
static int get_BSD_process_list(kinfo_proc **procList, size_t *procCount) {
  int err;
  kinfo_proc *result;
  bool done;
  static const int name[] = {CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0};
  // Declaring name as const requires us to cast it when passing it to
  // sysctl because the prototype doesn't include the const modifier.
  size_t length;

  //    assert( procList != NULL);
  //    assert(*procList == NULL);
  //    assert(procCount != NULL);

  *procCount = 0;

  // We start by calling sysctl with result == NULL and length == 0.
  // That will succeed, and set length to the appropriate length.
  // We then allocate a buffer of that size and call sysctl again
  // with that buffer.  If that succeeds, we're done.  If that fails
  // with ENOMEM, we have to throw away our buffer and loop.  Note
  // that the loop causes use to call sysctl with NULL again; this
  // is necessary because the ENOMEM failure case sets length to
  // the amount of data returned, not the amount of data that
  // could have been returned.

  result = NULL;
  done = false;
  do {
    assert(result == NULL);

    // Call sysctl with a NULL buffer.
    length = 0;
    err = sysctl((int *)name, (sizeof(name) / sizeof(*name)) - 1, NULL, &length,
                 NULL, 0);
    if (err == -1) {
      err = errno;
    }

    // Allocate an appropriately sized buffer based on the results
    // from the previous call.
    if (err == 0) {
      result = malloc(length);
      if (result == NULL) {
        err = ENOMEM;
      }
    }

    // Call sysctl again with the new buffer.  If we get an ENOMEM
    // error, toss away our buffer and start again.
    if (err == 0) {
      err = sysctl((int *)name, (sizeof(name) / sizeof(*name)) - 1, result,
                   &length, NULL, 0);
      if (err == -1) {
        err = errno;
      }
      if (err == 0) {
        done = true;
      } else if (err == ENOMEM) {
        assert(result != NULL);
        free(result);
        result = NULL;
        err = 0;
      }
    }
  } while (err == 0 && !done);

  // Clean up and establish post conditions.
  if (err != 0 && result != NULL) {
    free(result);
    result = NULL;
  }
  *procList = result;
  if (err == 0) {
    *procCount = length / sizeof(kinfo_proc);
  }

  assert((err == 0) == (*procList != NULL));
  return err;
}

int process_list_processes(process_list_t **result) {
  char pathbuf[PROC_PIDPATHINFO_MAXSIZE];
  int ret;

  kinfo_proc *proc_list = NULL;
  size_t proc_count = 0;

#ifdef COLLECT_STATS
  extern stats_t stats;
  struct timeval tv1;
  gettimeofday(&tv1, NULL);
#endif

  get_BSD_process_list(&proc_list, &proc_count);

  for (unsigned int i = 0; i < proc_count; i++) {
    struct kinfo_proc *process = &proc_list[i];
    // struct passwd *user = getpwuid(process->kp_eproc.e_ucred.cr_uid);

    struct proc_taskinfo task_info;
    ret = proc_pidinfo(process->kp_proc.p_pid, PROC_PIDTASKINFO, 0, &task_info,
                       sizeof(task_info));
    int ret_path =
        proc_pidpath(process->kp_proc.p_pid, pathbuf, sizeof(pathbuf));
    process_t *to_add = calloc(1, sizeof(process_t));
    to_add->tgid = process->kp_proc.p_pid;
    to_add->name = NULL;
    if (ret_path > 0) {
      to_add->cmdline = strdup(pathbuf);
    } else {
      to_add->cmdline = strdup("");
    }
    to_add->user = process->kp_eproc.e_ucred.cr_uid;
    /*
    if (user != NULL) {
      printf("uid: %d, name: %s\n", process->kp_eproc.e_ucred.cr_uid,
             user->pw_name);
    } else {
    */
    to_add->threads_num = task_info.pti_threadnum;
    // to_add->threads_run = task_info.pti_numrunning;
    to_add->vms = task_info.pti_virtual_size / 1024;
    to_add->rss = task_info.pti_resident_size / 1024;
    to_add->utime = task_info.pti_total_user;
    to_add->stime = task_info.pti_total_system;

    LL_APPEND((*result)->processes, to_add);
    // fields of taskInfo:
    //          uint64_t        pti_virtual_size;   /* virtual memory size
    //          (bytes) */
    //          uint64_t        pti_resident_size;  /* resident memory size
    //          (bytes) */
    //          uint64_t        pti_total_user;     /* total time */
    //          uint64_t        pti_total_system;
    //          uint64_t        pti_threads_user;   /* existing threads only
    //          */
    //          uint64_t        pti_threads_system;
    //          int32_t         pti_policy;     /* default policy for new
    //          threads */
    //          int32_t         pti_faults;     /* number of page faults */
    //          int32_t         pti_pageins;        /* number of actual
    //          pageins
    //          */
    //          int32_t         pti_cow_faults;     /* number of
    //          copy-on-write
    //          faults */
    //          int32_t         pti_messages_sent;  /* number of messages
    //          sent
    //          */
    //          int32_t         pti_messages_received;  /* number of
    //          messages
    //          received */
    //          int32_t         pti_syscalls_mach;  /* number of mach system
    //          calls */
    //          int32_t         pti_syscalls_unix;  /* number of unix system
    //          calls */
    //          int32_t         pti_csw;            /* number of context
    //          switches */
    //          int32_t         pti_threadnum;      /* number of threads in
    //          the
    //          task */
    //          int32_t         pti_numrunning;     /* number of running
    //          threads
    //          */
    //          int32_t         pti_priority;       /* task priority*/
  }
  free(proc_list);

  struct timeval tv;
  gettimeofday(&tv, NULL);
  (*result)->timestamp = 1000000 * tv.tv_sec + tv.tv_usec;

#ifdef COLLECT_STATS
  uint64_t t1 = 1000000 * tv1.tv_sec + tv1.tv_usec;
  stats.ps_dur = (*result)->timestamp - t1;
  stats.ps_times_count += 1;
  stats.ps_last_count = proc_count;
#endif

  return ret;
}
