## Process this file with automake to produce Makefile.in

AUTOMAKE_OPTIONS = foreign dist-zip 

SUBDIRS = lib include doc examples debian python

EXTRA_DIST = autogen.sh

DISTCLEANFILES = config.cache
# DISTCLEANFILES = config.guess config.sub 

debug:
	$(MAKE) all CFLAGS="@DEBUG@"

profile:
	$(MAKE) all CFLAGS="@PROFILE@"
