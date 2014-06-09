lib_LTLIBRARIES=libpsrthyme.la
nobase_include_HEADERS=psrthyme.h psrthyme/psrthyme.hpp


AM_CPPFLAGS=-I$(top_srcdir)/src/boost/

libpsrthyme_la_CPPFLAGS = $(AM_CPPFLAGS) -fPIC @T2TOOLKIT_CFLAGS@ 
libpsrthyme_la_LIBADD =@T2TOOLKIT_LIBS@ 

cppfiles = PsrthymeTemplate.cpp PsrthymeFitter.cpp \
		   PsrthymeComponent.cpp PsrthymeTemplateSection.cpp \
		   SparseList.cpp VonMisesComponent.cpp \
		   PsrthymeResult.cpp PsrthymeProfile.cpp \
		   PsrthymeTelescope.cpp PsrthymeArchive.cpp \
		   PsrthymeMatrix.cpp debug.cpp

hppfiles = PsrthymeMain.hpp \
		   PsrthymeTemplate.hpp PsrthymeFitter.hpp \
		   PsrthymeComponent.hpp PsrthymeTemplateSection.hpp \
		   SparseList.hpp VonMisesComponent.hpp \
		   PsrthymeResult.hpp PsrthymeProfile.hpp \
		   PsrthymeTelescope.hpp PsrthymeArchive.hpp \
		   PsrthymeMatrix.hpp debug.hpp

cfiles = 
hfiles =
makeheaders_files=$(cppfiles) $(cfiles) $(top_srcdir)/src/mjk_utils/util.h.in
BUILT_SOURCES=$(hppfiles) psrthyme.h psrthyme/psrthyme.hpp
clean-local:
	rm -f $(BUILT_SOURCES)

if HAVE_PGPLOT
cppfiles += PsrthymePlotter.cpp
hppfiles += PsrthymePlotter.hpp
libpsrthyme_la_CPPFLAGS += @PGPLOT_CFLAGS@ -I$(top_srcdir)/src/c++pgplot/
libpsrthyme_la_LIBADD +=@PGPLOT_LIBS@ $(top_builddir)/src/c++pgplot/libc++pgplot.la
endif

libpsrthyme_la_SOURCES = $(hfiles) $(hppfiles) $(cfiles) $(cppfiles)

if HAVE_LAPACK
libpsrthyme_la_LIBADD+=@LIBS_LAPACK@
endif

if HAVE_BLAS
libpsrthyme_la_LIBADD+=@BLAS_LIBS@
endif
.PHONY: makeheaders

makeheaders:
	make -C .. makeheaders

psrthyme.h: makeheaders $(makeheaders_files)
	$(top_builddir)/src/makeheaders -H $(makeheaders_files) > psrthyme.h
psrthyme/psrthyme.hpp: makeheaders $(makeheaders_files)
	mkdir -p psrthyme
	$(top_builddir)/src/makeheaders -h $(makeheaders_files) > psrthyme/psrthyme.hpp

headerfiles: makeheaders $(makeheaders_files)
	$(top_builddir)/src/makeheaders $(makeheaders_files)

$(hppfiles): headerfiles
$(hfiles): headerfiles

