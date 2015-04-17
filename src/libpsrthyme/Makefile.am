lib_LTLIBRARIES=libpsrthyme.la
nobase_include_HEADERS=psrthyme.h psrthyme/psrthyme.hpp

SUBDIRS=levmar

AM_CPPFLAGS=-I$(top_srcdir)/src/boost/ -DLOG_OUTDIR=stderr

libpsrthyme_la_CPPFLAGS = -I$(top_builddir)/src/libpsrthyme $(AM_CPPFLAGS) -fPIC @T2TOOLKIT_CFLAGS@ @PSRCHIVE_CFLAGS@ @FFTW_CFLAGS@
libpsrthyme_la_LIBADD =@T2TOOLKIT_LIBS@ @PSRCHIVE_LIBS@ @FFTW_LIBS@

cppfiles = Psrthyme.cpp \
		   PsrthymeTemplate.cpp PsrthymeFitter.cpp \
		   PsrthymeComponent.cpp PsrthymeTemplateSection.cpp \
		   SparseList.cpp VonMisesComponent.cpp \
		   PsrthymeResult.cpp PsrthymeProfile.cpp \
		   PsrthymeTelescope.cpp PsrthymeArchive.cpp \
		   PsrthymeToA.cpp ExpModGaussComponent.cpp \
		   PsrthymeMatrix.cpp debug.cpp PsrthymeLMFitter.cpp \
		   LevMar.cpp PsrthymeGenericFitter.cpp convolve.cpp \
		   SincFitter.cpp PsrthymeTemplateEditor.cpp

hppfiles = Psrthyme.hpp \
		   PsrthymeTemplate.hpp PsrthymeFitter.hpp \
		   PsrthymeComponent.hpp PsrthymeTemplateSection.hpp \
		   SparseList.hpp VonMisesComponent.hpp \
		   PsrthymeResult.hpp PsrthymeProfile.hpp \
		   PsrthymeTelescope.hpp PsrthymeArchive.hpp \
		   PsrthymeToA.hpp ExpModGaussComponent.hpp \
		   PsrthymeMatrix.hpp debug.hpp PsrthymeLMFitter.hpp \
		   LevMar.hpp PsrthymeGenericFitter.hpp convolve.hpp \
		   SincFitter.hpp PsrthymeTemplateEditor.hpp

cfiles =  mjk_clock.c
hfiles = mjk_clock.h

makeheaders_files= $(hppfiles:%.hpp=$(srcdir)/%.cpp) $(hfiles:%.h=$(srcdir)/%.c)
makeheaders_cmd = $(join $(hppfiles:%.hpp=$(srcdir)/%.cpp\:) $(hfiles:%.h=$(srcdir)/%.c\:), $(hppfiles) $(hfiles))

makeheaders_files+=$(top_srcdir)/src/mjk_utils/util.h.in
makeheaders_cmd+=$(top_srcdir)/src/mjk_utils/util.h.in:

BUILT_SOURCES=$(builddir)/madeheaders
madeheaders: $(makeheaders_files)
	make -C .. makeheaders
	$(top_builddir)/src/makeheaders $(makeheaders_cmd)
	touch madeheaders

$(hfiles) $(hppfiles): | madeheaders

created=$(hppfiles) $(hfiles) $(helpfiles) madeheaders

clean-local:
	rm -f $(created)
	


EXTRA_DIST=glob.cpp
if HAVE_PGPLOT
cppfiles += PsrthymePlotter.cpp
hppfiles += PsrthymePlotter.hpp
libpsrthyme_la_CPPFLAGS += @PGPLOT_CFLAGS@ -I$(top_srcdir)/src/c++pgplot/
libpsrthyme_la_LIBADD +=@PGPLOT_LIBS@ $(top_builddir)/src/c++pgplot/libc++pgplot.la
endif

libpsrthyme_la_SOURCES = $(hfiles) $(hppfiles) $(cfiles) $(cppfiles)

libpsrthyme_la_LIBADD+=levmar/liblevmar.la

if HAVE_LAPACK
libpsrthyme_la_LIBADD+=@LIBS_LAPACK@
endif

if HAVE_BLAS
libpsrthyme_la_LIBADD+=@BLAS_LIBS@
endif


psrthyme.h: madeheaders $(makeheaders_files)
	$(top_builddir)/src/makeheaders -H $(makeheaders_files) > psrthyme.h
psrthyme/psrthyme.hpp: madeheaders $(makeheaders_files)
	mkdir -p psrthyme
	echo "#ifndef _PSRTHYME_HPP" > psrthyme/psrthyme.hpp
	echo "#define _PSRTHYME_HPP" >> psrthyme/psrthyme.hpp
	$(top_builddir)/src/makeheaders -h $(makeheaders_files) >> psrthyme/psrthyme.hpp
	echo "#endif //_PSRTHYME_HPP" >> psrthyme/psrthyme.hpp


