AC_INIT([pandaresampler], [0.2.0])
AC_CONFIG_SRCDIR([include/pandaresampler.hh])
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

dnl
dnl zita resampler
dnl
AC_DEFUN([AC_ZITA_REQUIREMENTS],
[
    AC_CHECK_LIB(zita-resampler, _Z28zita_resampler_major_versionv,[],
      [
        AC_MSG_ERROR([You need to install libzita-resampler to build tests for this package.])
      ]
    )
])

dnl -------------------- building tests is optional -------------------------
AC_ARG_WITH([tests], [AS_HELP_STRING([--with-tests], [build test code])], [], [with_tests=no])
if test "x$with_tests" != "xno"; then
  PKG_CHECK_MODULES(FFTW, [fftw3 fftw3f])
  AC_ZITA_REQUIREMENTS
fi
AM_CONDITIONAL([COND_WITH_TESTS], [test "x$with_tests" != "xno"])
dnl -------------------------------------------------------------------------

AC_ARG_ENABLE(asan,AS_HELP_STRING([--enable-asan], [enable address sanitizer]),
[
asan_flags="-fno-omit-frame-pointer -fstack-protector-all -fno-inline -g -DG_ENABLE_DEBUG -fsanitize=address -fno-common"
CXXFLAGS="$CXXFLAGS $asan_flags"
])

AC_ARG_ENABLE(debug-cxx,AS_HELP_STRING([--enable-debug-cxx], [setup compiler flags to do C++ STL debug checks]),
[
  CXXFLAGS="$CXXFLAGS -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC"
])

# Less cluttered build output
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_CONFIG_FILES([Makefile filter-design/Makefile
                 include/Makefile include/pandaresampler/Makefile include/pandaresampler/hiir/Makefile
                 lib/Makefile tests/Makefile pandaresampler.pc])
AC_OUTPUT

echo
echo "---------------------------------------------------------------------------"
echo "$PACKAGE_NAME $PACKAGE_VERSION"
echo "---------------------------------------------------------------------------"
echo "  Build static library . .: $enable_static"
echo "  Build shared library . .: $enable_shared"
echo "  Tests . . . . . . . . . : $with_tests (use --with-tests to enable)"
if test "x$asan_flags" != "x"; then
  echo "  Address Sanitizer  . . .: $asan_flags"
fi

echo "  Install prefix . . . . .: $prefix"
echo
echo "Now type 'make', followed by 'make install' as root."
echo
