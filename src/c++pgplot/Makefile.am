lib_LTLIBRARIES=libcxxpgplot.la
include_HEADERS = cxxpgplot.h
noinst_PROGRAMS = cxxpgplot_test


AM_CPPFLAGS=-I../boost/

libcxxpgplot_la_CFLAGS = -fPIC @T2TOOLKIT_CFLAGS@ 
libcxxpgplot_la_LIBADD =@T2TOOLKIT_LIBS@ 

cppfiles = PgPlot.cpp PgPlotPane.cpp PgPlotData.cpp PgPlotText.cpp

hppfiles = PgPlot.hpp PgPlotPane.hpp PgPlotData.hpp PgPlotText.hpp

makeheaders_files=$(cppfiles) 
BUILT_SOURCES=$(hppfiles) cxxpgplot.h
clean-local:
	rm -f $(BUILT_SOURCES)

libcxxpgplot_la_SOURCES = $(hfiles) $(hppfiles) $(cfiles) $(cppfiles)

cxxpgplot_test_SOURCES = cxxpgplot_test.cpp cxxpgplot.h
cxxpgplot_test_LDADD = @PGPLOT_LIBS@ libcxxpgplot.la


cxxpgplot.h: ../makeheaders $(makeheaders_files)
	../makeheaders -h $(makeheaders_files) > cxxpgplot.h
headerfiles: ../makeheaders $(makeheaders_files)
	../makeheaders $(makeheaders_files)


$(hppfiles): headerfiles
$(hfiles): headerfiles

