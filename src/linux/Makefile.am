AM_CFLAGS = $(MORE_CFLAGS) -I$(abs_top_srcdir)/lib $(STATS_CFLAGS)
AM_CPPFLAGS = $(MORE_CPPFLAGS)
AM_LDFLAGS = $(MORE_LDFLAGS)

lib_LTLIBRARIES = libhost.la
libhost_la_SOURCES = linux_process.c linux_process.h
libhost_la_CFLAGS  = $(AM_CFLAGS) @PROCPS_CFLAGS@
libhost_la_LDFLAGS = $(AM_LDFLAGS) @PROCPS_LIBS@
