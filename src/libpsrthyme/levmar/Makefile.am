noinst_LTLIBRARIES=liblevmar.la

liblevmar_la_SOURCES=lm.c Axb.c misc.c lmlec.c lmbc.c lmblec.c lmbleic.c


if HAVE_LAPACK
if HAVE_BLAS
liblevmar_la_LIBADD=@LIBS_LAPACK@ @BLAS_LIBS@
endif
endif







