#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT(fedit2, [2020.08], akky@users.sourceforge.jp)

AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])

#AM_INIT_AUTOMAKE([gnu check-news])
AM_INIT_AUTOMAKE([gnu])


AC_LANG([C++])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

#AC_DISABLE_STATIC
#AC_PROG_LIBTOOL
#AC_SUBST(LIBTOOL_DEPS)

# Checks for libraries.

# ----------------------------------------------------------
# check Qt
AC_ARG_ENABLE(qt4,
              AC_HELP_STRING([--enable-qt4],
                             [use Qt4 tool kit instead of Qt5 (default no)]))
AC_ARG_ENABLE(gl,
              AC_HELP_STRING([--enable-gl],
                             [enable GLWidget for Qt (default yes)]))

if test "x$enable_qt4" == "xyes"; then

  AC_MSG_NOTICE(enabled Qt4)

  QTMODULES="QtCore QtGui QtNetwork"

  if test "x$enable_gl" == "xyes"; then
    AC_MSG_NOTICE(enabled GLWidget)
    CFLAGS="-DUSE_GLWIDGET $CFLAGS"
    CXXFLAGS="-DUSE_GLWIDGET $CXXFLAGS"
    QTMODULES="$QTMODULES QtOpenGL"
  fi

  AX_QT([4.3.0],[$QTMODULES])
else
  AC_MSG_NOTICE(enabled Qt5)

  QTMODULES="Qt5Core Qt5Gui Qt5Widgets Qt5Network"

  if test "x$enable_gl" == "xyes"; then
    AC_MSG_NOTICE(enabled GL5Widget)
    CFLAGS="-DUSE_GLWIDGET $CFLAGS"
    CXXFLAGS="-DUSE_GLWIDGET $CXXFLAGS"
    QTMODULES="$QTMODULES Qt5OpenGL"
  fi

  AX_QT([5.0.0],[$QTMODULES])
fi

if test x$have_qt != xyes ; then
  AC_MSG_ERROR([$QTMODULES could not be found.])
  AM_CONDITIONAL(BUILD_QT, [test "1" = "0"])
else
  CFLAGS="-fPIC $CFLAGS"
  CXXFLAGS="-fPIC $CXXFLAGS"
  AM_CONDITIONAL(BUILD_QT, [test "1" = "1"])
fi

# ----------------------------------------------------------
# check C++

AX_CXX_COMPILE_STDCXX_14(noext)

# ----------------------------------------------------------
# check boost
AX_BOOST_BASE([1.38.0])
AX_BOOST_SYSTEM
CPPFLAGS="$CPPFLAGS $BOOST_CPPFLAGS"
LDFLAGS="$LDFLAGS $BOOST_LDFLAGS"
LIBS="$LIBS $BOOST_SYSTEM_LIB"

# check librcsc path
AC_SUBST(LIBRCSCLIB)
LIBRCSCLIB="/usr/local/lib"
librcsc_prefix=""
AC_ARG_WITH(librcsc,
            [  --with-librcsc=PREFIX prefix where librcsc is installed (optional)],
            librcsc_prefix="$withval",
            librcsc_prefix="")
if test x$librcsc_prefix != x; then
  CPPFLAGS="$CPPFLAGS -I$librcsc_prefix/include"
  LDFLAGS="$LDFLAGS -L$librcsc_prefix/lib"
  LIBRCSCLIB="$librcsc_prefix/lib"
else
  for librcsc_path_tmp in $HOME/.local $HOME/local $HOME/rcss /opt/robocup /opt /opt/local ; do
    if test -d "$librcsc_path_tmp/include/rcsc" && test -r "$librcsc_path_tmp/include/rcsc/types.h" ; then
      CPPFLAGS="$CPPFLAGS -I$librcsc_path_tmp/include"
      LDFLAGS="$LDFLAGS -L$librcsc_path_tmp/lib"
      LIBRCSCLIB="$librcsc_path_tmp/lib"
      break;
    fi
  done
fi

# Checks for libraries.
AC_CHECK_LIB([m], [cos],
             [LIBS="-lm $LIBS"],
             [AC_MSG_ERROR([*** -lm not found! ***])])
AC_CHECK_LIB([rcsc], [main],
             [LIBS="-lrcsc $LIBS"],
             [AC_MSG_ERROR([*** -lrcsc not found! ***])])

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT32_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_HEADER_STDC
AC_CHECK_FUNCS([sqrt rint])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 tool/Makefile
                 tool/dttrainer/Makefile])

AC_OUTPUT
