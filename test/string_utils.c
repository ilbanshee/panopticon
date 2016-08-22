#include <assert.h>

#include "utils.h"

int main() {

  char* test0[] = {"a", "simple", "test", NULL};
  assert(11 == strv_length(&test0));

  char* test1[] = {"another\n", "test", NULL};
  assert(12 == strv_length(&test1));
  return 0;
}
