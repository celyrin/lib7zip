AC_INIT([lib7zip], [2.0.0], [jingnan.si@gmail.com])
AC_CONFIG_SRCDIR(Lib7Zip/lib7zip.h)
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AC_CONFIG_MACRO_DIR([m4])

AC_CANONICAL_HOST

AC_PROG_CC
AC_PROG_CXX
AC_C_CONST
AC_C_INLINE
AC_HEADER_STDBOOL

# Ensure C++11 support
CXXFLAGS="$CXXFLAGS -std=c++11"
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])

AC_PROG_RANLIB
LT_INIT([dlopen win32-dll])

AC_LANG(C++)

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_CHECK_HEADERS([stddef.h stdlib.h string.h])
AC_CHECK_HEADERS([fcntl.h stddef.h string.h unistd.h dlfcn.h dirent.h])
AC_CHECK_FUNCS([memset memmove])
AC_CHECK_FUNCS([sqrt])

dnl check if win32 system
AC_COMPILE_IFELSE([AC_LANG_PROGRAM(
				[[#ifndef _WIN32
				      error: This isn't WIN32!
		     #endif
     		]])],
                        [is_win32=yes],
                        [is_win32=no])

AM_CONDITIONAL(WIN32, test x$is_win32 = xyes)

dnl check for 7zip source
AC_ARG_VAR([SEVENZIP_SOURCE_DIR], [7zip source directory, must be set before compile source])

if test "x$SEVENZIP_SOURCE_DIR" = "x"; then
AC_MSG_FAILURE(SEVENZIP_SOURCE_DIR is not set)
fi

if test ! -d "$SEVENZIP_SOURCE_DIR/C"; then
AC_MSG_FAILURE($SEVENZIP_SOURCE_DIR/C is invalid)
fi

if test ! -d "$SEVENZIP_SOURCE_DIR/CPP"; then
AC_MSG_FAILURE(SEVENZIP_SOURCE_DIR is invalid)
fi

dnl create link for 7zip source  
AC_CONFIG_COMMANDS([sevenzip_src_cpp_link], [rm -rf includes/CPP && ln -sf $SEVENZIP_SOURCE_DIR/CPP includes/CPP])
AC_CONFIG_COMMANDS([sevenzip_src_c_link], [rm -rf includes/C && ln -sf $SEVENZIP_SOURCE_DIR/C includes/C])

AC_CONFIG_FILES([
	Makefile
	includes/Makefile
	Lib7Zip/Makefile
	Test7Zip/Makefile
])

AC_OUTPUT
