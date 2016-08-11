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
#include <duktape/duktape.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, const char *argv[]) {
  duk_context *ctx = NULL;
  char *line = strdup("I like *Sam & Max*.");
  int retval = 0;

  ctx = duk_create_heap_default();
  if (!ctx) {
    printf("Failed to create a Duktape heap.\n");
    exit(1);
  }

  if (duk_peval_file(ctx, SCRIPTPATH "/test/js_process.js") != 0) {
    printf("Error: %s\n", duk_safe_to_string(ctx, -1));
    retval = 1;
    goto finished;
  }
  duk_pop(ctx); /* ignore result */

  duk_push_global_object(ctx);
  duk_get_prop_string(ctx, -1 /*index*/, "processLine");
  duk_push_string(ctx, line);
  if (duk_pcall(ctx, 1 /*nargs*/) != 0) {
    printf("Error: %s\n", duk_safe_to_string(ctx, -1));
    retval = 1;
  } else {
    retval =
        strcmp("I like <b>Sam &#38; Max</b>.", duk_safe_to_string(ctx, -1));
  }
  duk_pop(ctx); /* pop result/error */

finished:
  duk_destroy_heap(ctx);

  exit(retval);
}
