bin_PROGRAMS=psrthyme
lib_LTLIBRARIES=libpsrthyme.la
include_HEADERS = psrthyme.h
noinst_PROGRAMS=makeheaders

if HAVE_CFITSIO
endif

psrthyme_LDADD=libpsrthyme.la
psrthyme_SOURCES=psrthyme-main.cpp mjk_cmd.c

AM_CPPFLAGS=-I./boost/

libpsrthyme_la_CFLAGS = -fPIC @TEMPO2_CFLAGS@ 
libpsrthyme_la_LIBADD =@TEMPO2_LIBS@ 

cppfiles = psrthyme-template.cpp psrthyme-fitter.cpp \
		   psrthyme-component.cpp psrthyme-profile.cpp \
		   sparselist.cpp VonMisesComponent.cpp \
		   psrthyme-result.cpp psrthyme-observation.cpp \
		   psrthyme-matrix.cpp

hppfiles = psrthyme-main.hpp \
		   psrthyme-template.hpp psrthyme-fitter.hpp \
		   psrthyme-component.hpp psrthyme-profile.hpp \
		   sparselist.hpp VonMisesComponent.hpp \
		   psrthyme-result.hpp psrthyme-observation.hpp \
		   psrthyme-matrix.hpp

cfiles = 
hfiles =
makeheaders_files=$(cppfiles) $(cfiles) $(psrthyme_SOURCES) util.h.in
BUILT_SOURCES=$(hppfiles) psrthyme.h
clean-local:
	rm $(BUILT_SOURCES)

if HAVE_PGPLOT
cppfiles += psrthyme-plotter.cpp
hppfiles += psrthyme-plotter.hpp
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
	./makeheaders psrthyme-main.cpp $(makeheaders_files)


$(hppfiles): headerfiles
$(hfiles): headerfiles

