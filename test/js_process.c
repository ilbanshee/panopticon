/* processline.c */
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
  } else {
    retval =
        strcmp("I like <b>Sam &#38; Max</b>.", duk_safe_to_string(ctx, -1));
  }
  duk_pop(ctx); /* pop result/error */

finished:
  duk_destroy_heap(ctx);

  exit(retval);
}
