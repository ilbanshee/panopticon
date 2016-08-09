AM_CFLAGS = $(MORE_CFLAGS)
AM_CPPFLAGS = $(MORE_CPPFLAGS)
AM_LDFLAGS = $(MORE_LDFLAGS)

bin_PROGRAMS = panopticon
dist_man_MANS = panopticon.8

## TODO:5000 When you want to add more files, add them below.
## TODO:5000 Automake will find dependencies by itself. Run
## TODO:5000 ./autogen.sh after modifying this file.

panopticon_SOURCES  = log.c log.h panopticon.h panopticon.c
panopticon_CFLAGS   = $(AM_CFLAGS) @JANSSON_CFLAGS@
panopticon_LDFLAGS  = $(AM_LDFLAGS) @JANSSON_LIBS@

## TODO:2004 Each time you have used `PKG_CHECK_MODULES` macro
## TODO:2004 in `configure.ac`, you get two variables that
## TODO:2004 you can substitute like above.
