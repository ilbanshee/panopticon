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
 * all
 * copies or substantial portions of the Software.
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
