noinst_PROGRAMS=psrthyme-test
bin_PROGRAMS=pt-toa pt-tmpl

psrthyme_test_SOURCES=psrthyme-test.cpp mjk_cmd.c

hppfiles=psrthyme-test.hpp pt-toa.hpp pt-tmpl.hpp
hfiles=mjk_cmd.h
helpfiles=help/pt-toa.help help/pt-tmpl.help

pt_toa_SOURCES=pt-toa.cpp mjk_cmd.c $(hppfiles) $(helpfiles)
pt_tmpl_SOURCES=pt-tmpl.cpp mjk_cmd.c $(hppfiles) $(helpfiles)

makeheaders_files= $(hppfiles:%.hpp=$(srcdir)/%.cpp) $(hfiles:%.h=$(srcdir)/%.c)
makeheaders_cmd = $(join $(hppfiles:%.hpp=$(srcdir)/%.cpp\:) $(hfiles:%.h=$(srcdir)/%.c\:), $(hppfiles) $(hfiles))

madeheaders: $(makeheaders_files)
	make -C .. makeheaders
	$(top_builddir)/src/makeheaders $(makeheaders_cmd)
	touch madeheaders

$(hfiles) $(hppfiles): | madeheaders


created=$(hppfiles) $(hfiles) $(helpfiles) madeheaders

clean-local:
	rm -f $(created)
	rm -f $(helpfiles)

BUILT_SOURCES=$(builddir)/madeheaders $(builddir)/$(helpfiles)
	

AM_CPPFLAGS=-I$(top_builddir)/src/libpsrthyme -I$(top_srcdir)/src/boost @FFTW_CFLAGS@

LDADD=$(top_builddir)/src/libpsrthyme/libpsrthyme.la $(top_builddir)/src/c++pgplot/libc++pgplot.la @FFTW_LIBS@

%.help: %.txt
	mkdir -p help
	xxd -i $< | sed -e 's: [^ ]*\(help_.*_txt\): \1:' > $@


