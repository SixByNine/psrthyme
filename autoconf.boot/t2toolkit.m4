#
# MJK_LIB_T2TOOLKIT([ACTION-IF-FOUND [,ACTION-IF-NOT-FOUND]])
#
# This m4 macro checks availability of the TEMPO2 Predictor library by
# Russell Edwards, George Hobbs, and Willem van Straten
#
# T2TOOLKIT_CFLAGS - autoconfig variable with flags required for compiling
# T2TOOLKIT_LIBS   - autoconfig variable with flags required for linking
# HAVE_T2TOOLKIT   - automake conditional
# HAVE_T2TOOLKIT   - pre-processor macro in config.h
#
# This macro tries to link a test program, using 
#
#    -I$TEMPO2/include -L$TEMPO2/lib -ltempo2pred
#
# Notice that the environment variable TEMPO2 is required.
#
# ----------------------------------------------------------
AC_DEFUN([MJK_LIB_T2TOOLKIT],
[
  AC_PROVIDE([MJK_LIB_T2TOOLKIT])

  AC_MSG_CHECKING([for TEMPO2 Toolkit library])

  T2TOOLKIT_CFLAGS="-I$TEMPO2/include"
  T2TOOLKIT_LIBS="-L$TEMPO2/lib -lt2toolkit -lm"

  ac_save_CXXFLAGS="$CXXFLAGS"
  ac_save_LIBS="$LIBS"

  AC_LANG_PUSH(C++)

  LIBS="$ac_save_LIBS $T2TOOLKIT_LIBS"
  CXXFLAGS="$ac_save_CXXFLAGS $T2TOOLKIT_CFLAGS"

  AC_TRY_LINK([#include "T2toolkit.h"],
              [TKsign_d (0.0,0.0);],
              have_t2toolkit=yes, have_t2toolkit=no)

  AC_MSG_RESULT($have_t2toolkit)

  LIBS="$ac_save_LIBS"
  CXXFLAGS="$ac_save_CXXFLAGS"

  AC_LANG_POP(C++)

  if test x"$have_t2toolkit" = xyes; then
    AC_DEFINE([HAVE_T2TOOLKIT], [1], [Define to 1 if you have the TEMPO2 toolkit library])
    [$1]
  else
    AC_MSG_WARN([TEMPO2 code will not be compiled])
    if test x"$TEMPO2" = x; then
      AC_MSG_WARN([Please set the TEMPO2 environment variable])
    fi
    T2TOOLKIT_CFLAGS=""
    T2TOOLKIT_LIBS=""
    [$2]
  fi

  AC_SUBST(T2TOOLKIT_CFLAGS)
  AC_SUBST(T2TOOLKIT_LIBS)
  AM_CONDITIONAL(HAVE_T2TOOLKIT, [test x"$have_t2toolkit" = xyes])

])

