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

#include "panopticon.h"

#include <getopt.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <uv.h>

#include "process.h"

extern const char *__progname;

static void usage(void) {
  fprintf(stderr, "Usage: %s [OPTIONS]\n", __progname);
  fprintf(stderr, "Version: %s\n", PACKAGE_STRING);
  fprintf(stderr, "\n");
  fprintf(stderr, " -d, --debug        be more verbose.\n");
  fprintf(stderr, " -h, --help         display help and exit\n");
  fprintf(stderr, " -v, --version      print version and exit\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "see manual page " PACKAGE "(8) for more information\n");
}

void signal_handler(uv_signal_t *handle, int signum) {
  printf("Signal received: %d\n", signum);
  uv_signal_stop(handle);
}

void timer_handler(uv_timer_t *handle) {
  printf("Timing out\n");
  exit(0);
}

int main(int argc, char *argv[]) {
  int debug = 1;
  int ch;

  static struct option long_options[] = {{"debug", no_argument, 0, 'd'},
                                         {"help", no_argument, 0, 'h'},
                                         {"version", no_argument, 0, 'v'},
                                         {0}};
  while (1) {
    int option_index = 0;
    ch = getopt_long(argc, argv, "hvdD:", long_options, &option_index);
    if (ch == -1)
      break;
    switch (ch) {
    case 'h':
      usage();
      exit(0);
      break;
    case 'v':
      fprintf(stdout, "%s\n", PACKAGE_VERSION);
      exit(0);
      break;
    case 'd':
      debug++;
      break;
    case 'D':
      log_accept(optarg);
      break;
    default:
      fprintf(stderr, "unknown option `%c'\n", ch);
      usage();
      exit(1);
    }
  }

  log_init(debug, __progname);

  process_list_t *p = process_list_new(geteuid());
  process_list_processes(&p);

  process_t *elt = NULL;
  int count = 0;

  DL_FOREACH(p->processes, elt) process_print(elt);
  DL_COUNT(p->processes, elt, count);

  printf("found %d processes\n", count);
  // test();

  uv_loop_t *loop = malloc(sizeof(uv_loop_t));
  uv_loop_init(loop);

  log_info("main", "hello world!");

  uv_signal_t sig_int;
  uv_signal_init(loop, &sig_int);
  uv_signal_start(&sig_int, signal_handler, SIGINT);

  uv_timer_t timer;
  uv_timer_init(loop, &timer);
  uv_timer_start(&timer, timer_handler, 1000, 0);

  uv_run(loop, UV_RUN_DEFAULT);

  uv_loop_close(loop);
  free(loop);

  return EXIT_SUCCESS;
}
