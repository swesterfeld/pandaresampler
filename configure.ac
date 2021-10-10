AC_INIT([pandaresampler], [0.1.0])
AC_CONFIG_SRCDIR([lib/pandaresampler.cc])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign])
AC_PROG_CXX
LT_INIT

PKG_CHECK_MODULES(FFTW, [fftw3 fftw3f])

# need c++11 mode
AX_CXX_COMPILE_STDCXX([11], [noext], [mandatory])

# use -Wall
AC_LANG_PUSH([C++])
AX_CHECK_COMPILE_FLAG([-Wall], [CXXFLAGS="$CXXFLAGS -Wall"])
AC_LANG_POP([C++])

dnl
dnl zita resampler
dnl
AC_DEFUN([AC_ZITA_REQUIREMENTS],
[
    AC_CHECK_LIB(zita-resampler, _Z28zita_resampler_major_versionv,[],
      [
        AC_MSG_ERROR([You need to install libzita-resampler to build this package.])
      ]
    )
])

AC_ZITA_REQUIREMENTS

# Less cluttered build output
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_CONFIG_FILES([Makefile filter-design/Makefile lib/Makefile tests/Makefile])
AC_OUTPUT
