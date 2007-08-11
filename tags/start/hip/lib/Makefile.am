## $Id$

INCLUDES = -I$(top_srcdir)/include -I$(top_builddir)

DEFS = @DEFS@ -DNOANALYSIS

lib_LTLIBRARIES = libmp3hip.la

libmp3hip_la_LDFLAGS = -version-info @H_LIB_CURRENT@:@H_LIB_REVISION@:@H_LIB_AGE@

libmp3hip_la_SOURCES = hip.c \
	common.c \
	dct64_i386.c \
	decode_i386.c \
	interface.c \
	layer1.c \
	layer2.c \
	layer3.c \
	tabinit.c

noinst_HEADERS = common.h \
	dct64_i386.h \
	decode_i386.h \
	huffman.h \
	interface.h \
	l2tables.h \
	layer1.h \
	layer2.h \
	layer3.h \
	mpg123.h \
	mpglib.h \
	tabinit.h

LCLINTFLAGS= \
	+posixlib \
	+showsummary \
	+showalluses \
	+whichlib \
	+forcehints \
	-fixedformalarray \
	+matchanyintegral \
	-Dlint

lclint.txt: ${libmp3hip_la_SOURCES} ${noinst_HEADERS}
	@lclint ${LCLINTFLAGS} ${INCLUDES} ${DEFS} ${libmp3hip_la_SOURCES} 2>&1 >lclint.txt || true

lclint: lclint.txt
	more lclint.txt

$(OBJECTS): libtool
libtool: $(LIBTOOL_DEPS)
	$(SHELL) $(top_builddir)/config.status --recheck
