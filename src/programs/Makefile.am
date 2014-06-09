noinst_PROGRAMS=psrthyme-test

psrthyme_test_SOURCES=psrthyme-test.cpp mjk_cmd.c

hppfiles=psrthyme-test.hpp
hfiles=mjk_cmd.h

makeheaders_files=mjk_cmd.c psrthyme-test.cpp
BUILT_SOURCES=$(hppfiles) $(hfiles)

clean-local:
	rm -f $(BUILT_SOURCES)

AM_CPPFLAGS=-I$(top_srcdir)/src/libpsrthyme -I$(top_srcdir)/src/boost

LDADD=$(top_builddir)/src/libpsrthyme/libpsrthyme.la $(top_builddir)/src/c++pgplot/libc++pgplot.la

headerfiles: $(makeheaders_files)
	$(top_builddir)/src/makeheaders $(makeheaders_files)

$(hppfiles): headerfiles
$(hfiles): headerfiles

