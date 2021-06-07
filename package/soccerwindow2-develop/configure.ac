#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.61)
AC_INIT([soccerwindow2], [2020.08], [akky@users.sourceforge.jp])

AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])

# ----------------------------------------------------------
#AM_INIT_AUTOMAKE([gnu check-news])
AM_INIT_AUTOMAKE([gnu])

# it is necessary to check c++ header files.
AC_LANG([C++])

# ----------------------------------------------------------
# Checks for programs.
#AC_PROG_AWK
AC_PROG_CC
AC_PROG_CXX
AC_PROG_RANLIB
AC_PROG_INSTALL

# ----------------------------------------------------------
# Checks for configure options
#AC_ARG_ENABLE(static,
#              [  --enable-static         statically linked],
#              [ enable_static=$enableval ],
#              [ enable_static=no ])
#if test "$enable_static" == "yes"; then
#  CFLAGS="-static $CFLAGS"
#  CXXFLAGS="-static $CXXFLAGS"
#fi

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

# check boost library for wx
#if test "x$enable_wx" == "xyes"; then
#  AX_BOOST_SIGNALS
#fi

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
  for librcsc_path_tmp in $HOME/local $HOME/.local /opt/local /usr/local /usr; do
    if test -d "$librcsc_path_tmp/include/rcsc" && test -r "$librcsc_path_tmp/include/rcsc/types.h" ; then
      CPPFLAGS="$CPPFLAGS -I$librcsc_path_tmp/include"
      LDFLAGS="$LDFLAGS -L$librcsc_path_tmp/lib"
      LIBRCSCLIB="$librcsc_path_tmp/lib"
      break;
    fi
  done
fi

# ----------------------------------------------------------
# Checks for libraries.
AC_CHECK_LIB([m], [cos],
             [LIBS="-lm $LIBS"],
             [AC_MSG_ERROR([*** -lm not found! ***])])
AC_CHECK_LIB([z], [deflate])

AC_CHECK_LIB([rcsc], [main],
             [LIBS="-lrcsc $LIBS"],
             [AC_MSG_ERROR([*** -lrcsc not found! ***])])

# ----------------------------------------------------------
# Checks for header files.
AC_CHECK_HEADERS([netinet/in.h],
                 break,
                 [AC_MSG_ERROR([*** netinet/in.h not found ***])])

# ----------------------------------------------------------
# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_PID_T
AC_STRUCT_TM

# ----------------------------------------------------------
# Checks for library functions.
AC_HEADER_STDC
AC_FUNC_STRFTIME
AC_FUNC_VPRINTF
#AC_CHECK_FUNCS([atexit],
#               break,
#               [AC_MSG_ERROR([*** atexit function not found!])])
AC_CHECK_FUNCS([floor memset pow rint sqrt strtol])
AC_CHECK_FUNC([rint],
              break,
              [AC_CHECK_LIB(m, rint, break,
                            [AC_MSG_ERROR([*** rint function not found!])]
                           )
              ]
            )

# ----------------------------------------------------------
#CFLAGS="$CFLAGS $SDL_CFLAGS"
#CXXFLAGS="$CXXFLAGS $SDL_CFLAGS"
#LIBS="$LIBS $SDL_LIBS"

# ----------------------------------------------------------
AC_CONFIG_FILES([Doxyfile
                 soccerwindow2.spec
                 Makefile
                 src/sswindow2
                 src/Makefile
                 src/model/Makefile
                 src/qt/Makefile],
                 [test -f src/sswindow2 && chmod +x src/sswindow2])
AC_OUTPUT
