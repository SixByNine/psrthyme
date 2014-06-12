lib_LTLIBRARIES=libc++pgplot.la
nobase_include_HEADERS = c++pgplot/pgplot.hpp
noinst_PROGRAMS = c++pgplot_test


AM_CPPFLAGS=-I$(top_srcdir)/src/boost/ -DLOG_OUTDIR=stderr

libc__pgplot_la_CFLAGS = -fPIC @T2TOOLKIT_CFLAGS@ 
libc__pgplot_la_LIBADD =@T2TOOLKIT_LIBS@ 

cppfiles = PgPlot.cpp PgPlotPane.cpp PgPlotData.cpp PgPlotText.cpp

hppfiles = PgPlot.hpp PgPlotPane.hpp PgPlotData.hpp PgPlotText.hpp

makeheaders_files=$(cppfiles) 
BUILT_SOURCES=$(hppfiles) c++pgplot/pgplot.hpp madeheaders
clean-local:
	rm -f $(BUILT_SOURCES)

libc__pgplot_la_SOURCES = $(hfiles) $(hppfiles) $(cfiles) $(cppfiles)

c__pgplot_test_SOURCES = c++pgplot_test.cpp c++pgplot.h
c__pgplot_test_LDADD = @PGPLOT_LIBS@ libc++pgplot.la


c++pgplot/pgplot.hpp: madeheaders $(makeheaders_files)
	mkdir -p c++pgplot
	$(top_builddir)/src/makeheaders -h $(makeheaders_files) > c++pgplot/pgplot.hpp

madeheaders: $(makeheaders_files)
	make -C .. makeheaders
	$(top_builddir)/src/makeheaders $(makeheaders_files)
	touch madeheaders


$(hppfiles): madeheaders 
$(hfiles): madeheaders

