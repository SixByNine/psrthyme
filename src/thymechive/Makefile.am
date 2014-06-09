lib_LTLIBRARIES=libthymechive.la
nobase_include_HEADERS=psrthyme/thymechive.hpp

AM_CPPFLAGS=-I$(top_srcdir)/src/boost -I$(top_srcdir)/src/libpsrthyme

libthymechive_la_CPPFLAGS = $(AM_CPPFLAGS) -fPIC @T2TOOLKIT_CFLAGS@ @PSRCHIVE_CFLAGS@
libthymechive_la_LIBADD =@T2TOOLKIT_LIBS@ @PSRCHIVE_LIBS@

cppfiles = ThymeChiveArchive.cpp

hppfiles = ThymeChiveArchive.hpp

makeheaders_files=$(cppfiles) $(cfiles) $(top_srcdir)/src/mjk_utils/util.h.in
BUILT_SOURCES=$(hppfiles) psrthyme/thymechive.hpp
clean-local:
	rm -f $(BUILT_SOURCES)

libthymechive_la_SOURCES = $(hfiles) $(hppfiles) $(cfiles) $(cppfiles)

.PHONY: makeheaders

makeheaders:
	make -C .. makeheaders

psrthyme/thymechive.hpp: makeheaders $(makeheaders_files)
	mkdir -p psrthyme
	$(top_builddir)/src/makeheaders -h $(makeheaders_files) > psrthyme/thymechive.hpp

headerfiles: makeheaders $(makeheaders_files)
	$(top_builddir)/src/makeheaders $(makeheaders_files)

$(hppfiles): headerfiles
$(hfiles): headerfiles

