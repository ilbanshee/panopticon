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
#ifndef UTILS_H_
#define UTILS_H_

/**
 * Frees a NULL-terminated array of strings, as well as each string it contains.
 * If str_array is NULL, this function simply returns.
 */
void strfreev(char **str_array);

/**
 * Returns the length of the given NULL-terminated string array str_array
 */
int strv_length(char **str_array);

/**
 * Joins a number of strings together to form one long string, with the optional
 * separator inserted between each of them. The returned string should be freed
 * with free().
 * If str_array has no items, the return value will be an empty string. If
 * str_array contains a single item, separator will not appear in the resulting
 * string.
 */
char *strjoinv(const char *separator, char **str_array);

/**
 *
 */
char **strv_copy(char **str_dest, char **str_src);


#endif // UTILS_H_
