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
#include "process.h"
#include <config.h>
#include <inttypes.h>
#include <stdio.h> // for printf

#ifdef HOST_DARWIN
#define TIME_TO_MIN 1000000000.0L
#endif

process_list_t *process_list_new(uid_t user_id) {
  process_list_t *res = calloc(1, sizeof(process_list_t));
  res->processes = NULL;
  res->users = NULL;
  res->current_id = user_id;
  return res;
}

void process_print(process_t *process) {
  printf("pid: %d, tn: %d, tr: %d, vms: %" PRIu64 ", rss: %" PRIu64
         ", cpu: %Lf\n",
         process->pid, process->threads_num, process->threads_run, process->vms,
         process->rss, (process->utime + process->stime) / TIME_TO_MIN);
}