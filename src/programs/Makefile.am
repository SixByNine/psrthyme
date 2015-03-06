noinst_PROGRAMS=psrthyme-test
bin_PROGRAMS=pt-toa

psrthyme_test_SOURCES=psrthyme-test.cpp mjk_cmd.c

hppfiles=psrthyme-test.hpp pt-toa.hpp
hfiles=mjk_cmd.h
helpfiles=help/pt-toa.help

pt_toa_SOURCES=pt-toa.cpp mjk_cmd.c $(hppfiles) $(helpfiles)

makeheaders_files= psrthyme-test.cpp pt-toa.cpp mjk_cmd.c
BUILT_SOURCES=$(hppfiles) $(hfiles) madeheaders $(helpfiles)

clean-local:
	rm -f $(BUILT_SOURCES)

AM_CPPFLAGS=-I$(top_srcdir)/src/libpsrthyme -I$(top_srcdir)/src/boost @FFTW_CFLAGS@

LDADD=$(top_builddir)/src/libpsrthyme/libpsrthyme.la $(top_builddir)/src/c++pgplot/libc++pgplot.la @FFTW_LIBS@


madeheaders: $(makeheaders_files)
	make -C .. makeheaders
	$(top_builddir)/src/makeheaders $(makeheaders_files)
	touch madeheaders


$(hppfiles): madeheaders 
$(hfiles): madeheaders

%.help: %.txt
	xxd -i $< > $@
