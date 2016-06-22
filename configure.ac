#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([libionsim], [0.0.1], [joelfred@slac.stanford.edu])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([-Wall -Werror -Woverride subdir-objects])
AC_CONFIG_SRCDIR([src/ionsim.h])

# Checks for libraries.
PKG_CHECK_MODULES([GSL], [gsl >= 1.15])
AX_LIB_HDF5()
if test "$with_hdf5" = "no"; then
        AC_MSG_ERROR([Unable to find HDF5, we need parallel HDF5.])
fi

# AC_CHECK_LIB([gflags], [], [HAVE_GFLAGS=1] [GFLAGS_LIBS="-lgflags"], AC_MSG_WARN([gflags is not installed.]))
GFLAGS_LIBS="-lgflags"
AC_SUBST(GFLAGS_LIBS,[$GFLAGS_LIBS])

MYLDADD="$HDF5_LIBS $GFLAGS_LIBS"
AC_SUBST(MYLDADD,[$MYLDADD])

MYAM_CPPFLAGS="$HDF5_CPPFLAGS $GFLAGS_CPPFLAGS"
AC_SUBST(MYAM_CPPFLAGS,[$MYAM_CPPFLAGS])

# Checks for programs.
if test "$HDF5_TYPE" = "parallel"; then
	AX_PROG_CXX_MPI
	AX_PROG_CC_MPI
elif test "$HDF5_TYPE" = "serial"; then
	AC_PROG_CC
	AC_PROG_CXX
else
	AC_MSG_ERROR([Didn't find serial or parallel HDF5])
fi
# AX_CXX_COMPILE_STDCXX_11(,[mandatory])
AX_CXX_COMPILE_STDCXX_0X
AM_PROG_AR
LT_INIT
AC_CONFIG_MACRO_DIRS([m4])

PKG_PROG_PKG_CONFIG
PKG_INSTALLDIR

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile
		 src/ionsim.pc
		])
AC_OUTPUT
