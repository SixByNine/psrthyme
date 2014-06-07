bin_PROGRAMS=psrthyme
lib_LTLIBRARIES=libpsrthyme.la
include_HEADERS = psrthyme.h
noinst_PROGRAMS=makeheaders

if HAVE_CFITSIO
endif

psrthyme_LDADD=libpsrthyme.la
psrthyme_SOURCES=PsrthymeMain.cpp mjk_cmd.c

AM_CPPFLAGS=-I./boost/

libpsrthyme_la_CFLAGS = -fPIC @T2TOOLKIT_CFLAGS@ 
libpsrthyme_la_LIBADD =@T2TOOLKIT_LIBS@ 

cppfiles = PsrthymeTemplate.cpp PsrthymeFitter.cpp \
		   PsrthymeComponent.cpp PsrthymeProfile.cpp \
		   sparselist.cpp VonMisesComponent.cpp \
		   PsrthymeResult.cpp PsrthymeObservation.cpp \
		   PsrthymeMatrix.cpp debug.cpp

hppfiles = PsrthymeMain.hpp \
		   PsrthymeTemplate.hpp PsrthymeFitter.hpp \
		   PsrthymeComponent.hpp PsrthymeProfile.hpp \
		   sparselist.hpp VonMisesComponent.hpp \
		   PsrthymeResult.hpp PsrthymeObservation.hpp \
		   PsrthymeMatrix.hpp debug.hpp

cfiles = 
hfiles =
makeheaders_files=$(cppfiles) $(cfiles) $(psrthyme_SOURCES) util.h.in
BUILT_SOURCES=$(hppfiles) psrthyme.h
clean-local:
	rm -f $(BUILT_SOURCES)

if HAVE_PGPLOT
SUBDIRS=c++pgplot
cppfiles += PsrthymePlotter.cpp
hppfiles += PsrthymePlotter.hpp
libpsrthyme_la_CFLAGS += @PGPLOT_CFLAGS@ 
libpsrthyme_la_LIBADD +=@PGPLOT_LIBS@ 
endif

libpsrthyme_la_SOURCES = $(hfiles) $(hppfiles) $(cfiles) $(cppfiles)

if HAVE_LAPACK
libpsrthyme_la_LIBADD+=@LIBS_LAPACK@
endif

if HAVE_BLAS
libpsrthyme_la_LIBADD+=@BLAS_LIBS@
endif

psrthyme.h: makeheaders $(makeheaders_files)
	./makeheaders -H $(makeheaders_files) > psrthyme.h
headerfiles: makeheaders $(makeheaders_files)
	./makeheaders PsrthymeMain.cpp $(makeheaders_files)


$(hppfiles): headerfiles
$(hfiles): headerfiles

