#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "../src/utils.h"

int main() {

  char* test0[] = {"a", "simple", "test", NULL};
  char* test1[] = {"another\n", "test", NULL};
  char* test2[] = {"one", "more", "long", "test", "array", NULL};

  char** p_test0 = (char**) &test0;
  char** p_test1 = (char**) &test1;

  assert(11 == strv_length(p_test0));
  assert(12 == strv_length(p_test1));

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
  cp = strv_copy(p_test1);
  assert(12 == strv_length(cp));
  strfreev(cp);

  return 0;
}
