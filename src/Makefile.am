AM_CFLAGS = $(MORE_CFLAGS)
AM_CPPFLAGS = $(MORE_CPPFLAGS)
AM_LDFLAGS = $(MORE_LDFLAGS)

bin_PROGRAMS = panopticon
dist_man_MANS = panopticon.8

panopticon_SOURCES  = log.c log.h panopticon.h panopticon.c process.h
if LINUX
panopticon_SOURCES += linux/linux_process.c linux/linux_process.h
endif
panopticon_CFLAGS   = $(AM_CFLAGS) @AVRO_CFLAGS@ @LIBUV_CFLAGS@
panopticon_LDFLAGS  = $(AM_LDFLAGS) @AVRO_LIBS@ @LIBUV_LIBS@
