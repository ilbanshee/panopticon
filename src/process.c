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

#if HOST_DARWIN == 1
#define TIME_TO_MIN 1000000000.0L
#endif

#if HOST_LINUX == 1
#include <sys/times.h>
#include <unistd.h>
#define TIME_TO_MIN (sysconf(_SC_CLK_TCK))
#endif

int pidcmp(process_t *a, process_t *b) { return (a->tgid > b->tgid); }

process_usage_list_t *process_get_usage(process_list_t *a_list,
                                        process_list_t *b_list) {
  process_usage_list_t *ret = calloc(1, sizeof(process_usage_list_t));
  ret->timestamp = b_list->timestamp;
  ret->measure_delta = b_list->timestamp - a_list->timestamp;

  process_t *it_a, *it_b;
  process_usage_t *to_add;

  it_a = a_list->processes;
  it_b = b_list->processes;
  while (it_a->next != NULL || it_b->next != NULL) {
    to_add = calloc(1, sizeof(process_usage_t));
    if (it_a->tgid < it_b->tgid) {
      to_add->tgid = it_a->tgid;
      to_add->next = NULL;
      to_add->state = STATE_NEW;
      LL_APPEND(ret->usages, to_add);
      it_a = it_a->next;
    } else if (it_a->tgid == it_b->tgid) {
      to_add->tgid = it_a->tgid;
      to_add->next = NULL;
      to_add->state = STATE_NORMAL;
      to_add->time_in_measure =
          it_b->utime - it_a->utime + it_b->stime - it_a->stime;
      LL_APPEND(ret->usages, to_add);
      it_a = it_a->next;
      it_b = it_b->next;
    } else {
      to_add->tgid = it_b->tgid;
      to_add->next = NULL;
      to_add->state = STATE_NEW;
      LL_APPEND(ret->usages, to_add);
      it_b = it_b->next;
    }
  }
  while (it_a->next != NULL) {
    to_add = calloc(1, sizeof(process_t));
    memcpy(to_add, it_a, sizeof(process_t));
    to_add->next = NULL;
    to_add->state = STATE_NEW;
    LL_APPEND(ret->usages, to_add);
    it_a = it_a->next;
  }
  while (it_b->next != NULL) {
    to_add = calloc(1, sizeof(process_t));
    memcpy(to_add, it_b, sizeof(process_t));
    to_add->next = NULL;
    to_add->state = STATE_NEW;
    LL_APPEND(ret->usages, to_add);
    it_b = it_b->next;
  }
  return ret;
}

process_list_t *process_list_new(uid_t user_id) {
  process_list_t *res = calloc(1, sizeof(process_list_t));
  res->processes = NULL;
  res->users = NULL;
  res->current_id = user_id;
  return res;
}

void process_usage_print(process_usage_t *process) {
  if (process->time_in_measure > 0) {
    printf("tgid: %d, state: %d, time: %" PRIu64 ", m: %Lf\n", process->tgid,
           process->state, process->time_in_measure,
           (long double)process->time_in_measure / TIME_TO_MIN);
  }
}

void process_print(process_t *process) {
  printf("tgid: %d, tn: %d, vms: %" PRIu64 ", rss: %" PRIu64 ", cpu: %Lf\n",
         process->tgid, process->threads_num, process->vms, process->rss,
         (long double)(process->utime + process->stime) / TIME_TO_MIN);
}
