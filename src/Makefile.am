AM_CFLAGS = $(MORE_CFLAGS) -I$(abs_top_srcdir)/lib $(STATS_CFLAGS)
AM_CPPFLAGS = $(MORE_CPPFLAGS)
AM_LDFLAGS = $(MORE_LDFLAGS)

bin_PROGRAMS = panopticon
dist_man_MANS = panopticon.8

panopticon_SOURCES  = log.c log.h panopticon.h panopticon.c \
	process.h process.c stats.c stats.h utils.h utils.c
panopticon_CFLAGS   = $(AM_CFLAGS) @AVRO_CFLAGS@ @LIBUV_CFLAGS@
if LINUX
panopticon_LDADD    = linux/libhost.la
endif
if DARWIN
panopticon_LDADD    = darwin/libhost.la
endif
panopticon_LDFLAGS  = $(AM_LDFLAGS) @AVRO_LIBS@ @LIBUV_LIBS@

EXTRA_DIST = mainpage.dox

lib_LTLIBRARIES = libtest.la
libtest_la_SOURCES = utils.c utils.h process.h process.c
