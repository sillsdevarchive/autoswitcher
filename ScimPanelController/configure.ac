#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([Scim Panelcontroller], [1.0.0], [tim_armstrong@sil.org])
AC_CONFIG_SRCDIR([src/scim_panelcontrol_client.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
LT_INIT

# Checks for libraries.
PKG_CHECK_MODULES(SCIM, scim >= 1.4.7)
AC_SUBST(SCIM_CFLAGS)
AC_SUBST(SCIM_LIBS)

# Checks for header files.
AC_PATH_X

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_HEADER_STDC

# Checks for library functions.
AC_CHECK_FUNCS([select])

AC_CONFIG_FILES([
	Makefile
	src/Makefile
	])
AC_OUTPUT
