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
#include "linux_process.h"

#include <proc/readproc.h>
#include <sys/time.h>

#include "../stats.h"
#include "../utils.h"

int test() { return 1; }
int process_list_processes(process_list_t **result) {
  proc_t proc_info;
#ifdef COLLECT_STATS
  size_t proc_count = 0;
  extern stats_t stats;
  struct timeval tv1;
  gettimeofday(&tv1, NULL);
#endif

  PROCTAB *proc =
      openproc(PROC_FILLMEM | PROC_FILLSTAT | PROC_FILLSTATUS | PROC_FILLCOM);

  memset(&proc_info, 0, sizeof(proc_info));
  while (readproc(proc, &proc_info) != NULL) {
    process_t *to_add = calloc(1, sizeof(process_t));
    to_add->tgid = proc_info.tgid;
    to_add->name = proc_info.cmd;
    if (proc_info.cmdline != NULL) {
      to_add->cmdline = strjoinv(" ", proc_info.cmdline);
    }
    to_add->vms = proc_info.vm_size;
    to_add->rss = proc_info.vm_rss;
    to_add->threads_num = proc_info.nlwp;

    to_add->utime = proc_info.utime;
    to_add->stime = proc_info.stime;

    to_add->user = proc_info.euid;
#ifdef COLLECT_STATS
    proc_count++;
#endif

    LL_APPEND((*result)->processes, to_add);
    // printf("%20s:\t%5ld\t%5lld\t%5lld\n", proc_info.cmd, proc_info.resident,
    //       proc_info.utime, proc_info.stime);
  }
  closeproc(proc);

  LL_SORT((*result)->processes, pidcmp);

  struct timeval tv;
  gettimeofday(&tv, NULL);
  (*result)->timestamp = 1000000 * tv.tv_sec + tv.tv_usec;

#ifdef COLLECT_STATS
  uint64_t t1 = 1000000 * tv1.tv_sec + tv1.tv_usec;
  stats.ps_dur = (*result)->timestamp - t1;
  stats.ps_times_count += 1;
  stats.ps_last_count = proc_count;
#endif

  return 1;
}
