noinst_LTLIBRARIES=liblevmar.la

liblevmar_la_SOURCES=lm.c Axb.c misc.c lmlec.c lmbc.c lmblec.c lmbleic.c

liblevmar_la_CPPFLAGS='-std=gnu11'

if HAVE_LAPACK
if HAVE_BLAS
liblevmar_la_LIBADD=@LIBS_LAPACK@ @BLAS_LIBS@
endif
endif







