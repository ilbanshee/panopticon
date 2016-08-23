#include <assert.h>
#include <stddef.h>

#include "../src/utils.h"

int main() {

  char* test0[] = {"a", "simple", "test", NULL};
  assert(11 == strv_length(&test0));

  char* test1[] = {"another\n", "test", NULL};
  assert(12 == strv_length(&test1));

  char* test2[] = {"one", "more", "long", "test", "array", NULL};

  char* res = strjoinv("", test0);
  assert(0 == strcmp(res, "asimpletest"));
  free(res);
  res = strjoinv(" ", test0);
  assert(0 == strcmp(res, "a simple test"));
  free(res);

  res = strjoinv("\n", test0);
  assert(0 == strcmp(res, "a\nsimple\ntest"));
  free(res);

  res = strjoinv("-->", test2);
  assert(0 == strcmp(res, "one-->more-->long-->test-->array"));
  free(res);

  res = strjoinv("\n", test1);
  assert(0 == strcmp(res, "another\n\ntest"));
  free(res);

  char** cp = NULL;
  cp = strv_copy(&test1);
  assert(12 == strv_length(cp));
  strfreev(cp);

  return 0;
}
