AM_CFLAGS = $(MORE_CFLAGS) -I$(abs_top_srcdir)/lib
AM_CPPFLAGS = $(MORE_CPPFLAGS)
AM_LDFLAGS = $(MORE_LDFLAGS)

check_PROGRAMS = js_process string_utils
TESTS = $(check_PROGRAMS)

js_process_SOURCES = js_process.c
js_process_CFLAGS  = $(AM_CFLAGS) -DSCRIPTPATH=\"$(abs_top_srcdir)\"
js_process_LDFLAGS = $(abs_top_builddir)/lib/duktape/libduktape.la
EXTRA_DIST = js_process.js

string_utils_SOURCES = string_utils.c
string_utils_LDADD   = $(abs_top_builddir)/src/libtest.la
