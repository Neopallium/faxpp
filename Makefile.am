AUTOMAKE_OPTIONS = foreign dist-zip
SUBDIRS = include
EXTRA_DIST = 

INCLUDES = -I$(top_srcdir)/include/ 

bin_PROGRAMS = test test2 test3
lib_LTLIBRARIES = libfaxpp.la

# Library version, see http://sourceware.org/autobook/autobook/autobook_91.html
libfaxpp_la_LDFLAGS = -version-info 0:0:0 -no-undefined

libfaxpp_la_SOURCES = \
src/xml_parser.c \
src/xml_tokenizer.c \
src/token.c \
src/transcode.c \
src/error.c \
src/char_classes.c \
src/tokenizer_states.h \
src/tokenizer_states.c \
src/cdata.c \
src/comment.c \
src/element.c \
src/pi.c \
src/reference.c \
src/xmldecl.c \
src/event.c \
src/buffer.c \
src/attr_states.h \
src/element_states.h

test_SOURCES = src/test.c
test_LDADD = libfaxpp.la

test2_SOURCES = src/test2.c
test2_LDADD = libfaxpp.la

test3_SOURCES = src/test3.c
test3_LDADD = libfaxpp.la

