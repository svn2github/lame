## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = foreign

INCLUDES = -I$(top_srcdir)/include

noinst_PROGRAMS = hip_example

LDFLAGS = -all-static
LDADD = ../lib/libmp3hip.la

hip_example_SOURCES = hip_example.c

debug:
	$(MAKE) all CFLAGS="@DEBUG@"

profile:
	$(MAKE) all CFLAGS="@PROFILE@"
