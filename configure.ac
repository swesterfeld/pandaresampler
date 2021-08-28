AC_INIT([pandaresampler], [0.1.0])
AC_CONFIG_SRCDIR([lib/pandaresampler.cc])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([foreign])
AC_PROG_CXX
AC_DISABLE_SHARED
LT_INIT([pic-only])

[MAJOR_VERSION=`echo $PACKAGE_VERSION | cut -d . -f 1`]
[MINOR_VERSION=`echo $PACKAGE_VERSION | cut -d . -f 2`]
[MICRO_VERSION=`echo $PACKAGE_VERSION | cut -d . -f 3`]

AC_SUBST(MAJOR_VERSION)
AC_SUBST(MINOR_VERSION)
AC_SUBST(MICRO_VERSION)

dnl construct libtool version info (see also https://www.gnu.org/software/libtool/manual/html_node/Updating-version-info.html)
dnl
dnl  - every time we change $MAJOR_VERSION or $MINOR_VERSION, ABI can be totally different
dnl  - every time we change $MICRO_VERSION, only compatible ABI additions are possible
dnl
dnl  - for code which was compiled against the same $MAJOR_VERSION and $MINOR_VERSION of the library,
dnl    * old binaries will run with newer $MICRO_VERSION (lt_revision)
dnl    * new binaries will not run with older $MICRO_VERSION (lt_revision, lt_age == 0)
dnl
lt_current=`expr $MAJOR_VERSION \* 100 + $MINOR_VERSION`
lt_revision=$MICRO_VERSION
lt_age=0
LT_VERSION_INFO=$lt_current:$lt_revision:$lt_age
AC_SUBST(LT_VERSION_INFO)

# use -Wall
AC_LANG_PUSH([C++])
AX_CHECK_COMPILE_FLAG([-Wall], [CXXFLAGS="$CXXFLAGS -Wall"])
AC_LANG_POP([C++])

# Less cluttered build output
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_CONFIG_FILES([Makefile lib/Makefile tests/Makefile])
AC_OUTPUT

echo
echo "---------------------------------------------------------------------------"
echo "$PACKAGE_NAME $PACKAGE_VERSION"
echo "---------------------------------------------------------------------------"
echo "  Build static library . .: $enable_static"
echo "  Build shared library . .: $enable_shared"
echo
if test "x$enable_shared" = "xyes"; then
  echo "NOTE: Shared libraries can cause problems if binaries created by different"
  echo "people are combined (for instance for an Plugin binary and a DAW binary"
  echo "which need different versions of pandaresampler)."
  echo ""
  echo "So using static libraries is the safer choice."
  echo ""
fi
