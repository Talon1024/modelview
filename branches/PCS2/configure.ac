dnl generic init
AC_INIT(pcs2.cpp)
AM_CONFIG_HEADER(config.h:config.h.in)
AM_INIT_AUTOMAKE(pcs2, 0.0.1)
dnl AC_PROG_MAKE_SET
dnl AC_ARG_PROGRAM
AC_PROG_INSTALL
AC_PROG_RANLIB

dnl check for programs we use
AC_PROG_CXX

AM_OPTIONS_WXCONFIG
AM_PATH_WXCONFIG(2.6.0,, AC_MSG_ERROR([
wxWidgets must be installed on your system but wx-config script
couldn't be found.
]), [core,gl])

CPPFLAGS="$CPPFLAGS $WX_CPPFLAGS -I/usr/local/include -L/usr/local/lib"
CXXFLAGS="$CXXFLAGS $WX_CXXFLAGS_ONLY -I/usr/local/include -L/usr/local/lib"

AC_SUBST(WX_LIBS)

AC_ARG_ENABLE(debug,
[  --enable-debug,  Enable debug build],
[
  debugbuild="y"
  CXXFLAGS="${CXXFLAGS} -ggdb"
])

AC_CHECK_FUNCS([filelength fstat strncmp strnicmp strncasecmp])

AC_CHECK_LIB(GL, glFinish,,AC_MSG_ERROR([
OpenGL library not detected!
]))

#AC_CHECK_HEADERS([IL/il.h IL/ilu.h IL/ilut.h])
#AC_CHECK_HEADER([stdio.h],
#                [AC_DEFINE([HAVE_STDIO_H])],
#                [AC_MSG_ERROR([Sorry, can't do anything for you])])

AC_CHECK_HEADER([IL/il.h], ,[AC_MSG_ERROR([
IL Devil library not detected!
])])

AC_CHECK_HEADER([IL/ilu.h],,[AC_MSG_ERROR([
ILU Devil library not detected!
])])

AC_CHECK_HEADER([IL/ilut.h], ,[AC_MSG_ERROR([
ILUT Devil library not detected!
])])

AC_CHECK_HEADERS([sys/types.h sys/stat.h unistd.h GL/gl.h])

dnl that's all, folks
AC_OUTPUT([
            Makefile
	    compat/Makefile
          ])

dnl Summarized output
echo
echo "***************************************"
echo

if [[ "$debugbuild" = "y" ]]; then
echo "Debug               Enabled"
else
echo "Debug               Disabled"
fi
