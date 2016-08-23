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
#include "utils.h"

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void strfreev(char **str_array) {
  if (str_array == NULL) {
    return;
  }
  char *tmp = *str_array;
  for (int i = 1; tmp != NULL; i++) {
    free(tmp);
    tmp = *(str_array + i);
  }
  free(str_array);
}

int strv_length(char **str_array) {
  char *tmp = *str_array;
  int len = 0;
  for (int i = 1; tmp != NULL; i++) {
    len += strlen(tmp);
    tmp = *(str_array + i);
  }
  return len;
}

char *strjoinv(const char *separator, char **str_array) {
  char *res = NULL;
  char *tmp = *str_array;
  int len = 0;
  int offset = 0;
  int sep_len = strlen(separator);
  for (int i = 1; tmp != NULL; i++) {
    len += strlen(tmp);
    /* string + separator + \0 */
    res = realloc(res, len + sep_len + 1);
    strcpy(res + offset, tmp);
    if (sep_len > 0 && *(str_array + i) != NULL) {
      strcpy(res + len, separator);
    }
    offset = len + sep_len;
    len += sep_len;
    tmp = *(str_array + i);
  }
  return res;
}

char **strv_copy(char **str_src) {
  char **str_dest = NULL;
  char *tmp = *str_src;
  int i = 0;
  while(tmp != NULL) {
    str_dest = realloc(str_dest, (i+1) * sizeof(char *));
    str_dest[i] = calloc(strlen(tmp)+1, sizeof(char));
    strcpy(str_dest[i], tmp);
    tmp = *(str_src + ++i);
  }
  str_dest = realloc(str_dest, (i+1) * sizeof(char *));
  str_dest[i] = NULL;
  return str_dest;
}
