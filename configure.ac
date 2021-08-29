AC_INIT([pandaresampler], [0.1.0])
AC_CONFIG_SRCDIR([lib/pandaresampler.cc])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign])
AC_PROG_CXX
LT_INIT

# need c++11 mode
AX_CXX_COMPILE_STDCXX([11], [noext], [mandatory])

# use -Wall
AC_LANG_PUSH([C++])
AX_CHECK_COMPILE_FLAG([-Wall], [CXXFLAGS="$CXXFLAGS -Wall"])
AC_LANG_POP([C++])

# Less cluttered build output
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_CONFIG_FILES([Makefile lib/Makefile tests/Makefile])
AC_OUTPUT
