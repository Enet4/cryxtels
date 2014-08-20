#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

PACKAGE=crystal-pixels
VERSION=1.1.0

AC_PREREQ([2.69])
AC_INIT([crystal-pixels], [1.1.0])
AC_CONFIG_SRCDIR([source/cryxtels.cpp])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX

# Use C++
AC_LANG(C++)

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdlib.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE

# Checks for library functions.
AC_CHECK_FUNCS([atexit memset sqrt strcasecmp strerror])

AC_CONFIG_FILES([Makefile source/Makefile])

AC_OUTPUT
