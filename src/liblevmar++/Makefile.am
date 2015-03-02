lib_LTLIBRARIES=liblevmar++.la
nobase_include_HEADERS = levmar++/levmar.hpp

AM_CPPFLAGS=-I$(top_srcdir)/src/boost/ -DLOG_OUTDIR=stderr

cppfiles = LevMar.cpp 
hppfiles = LevMar.hpp

makeheaders_files=$(cppfiles) 
BUILT_SOURCES=$(hppfiles) levmar++/levmar.hpp 
clean-local:
	rm -f $(BUILT_SOURCES)

liblevmar___la_SOURCES = $(hfiles) $(hppfiles) $(cfiles) $(cppfiles)

$(hppfiles): $(cppfiles)
	../makeheaders.py $(cppfiles)

levmar++/levmar.hpp: $(hppfiles)
	mkdir -p levmar++
	cat *.hpp >> levmar++/levmar.hpp
