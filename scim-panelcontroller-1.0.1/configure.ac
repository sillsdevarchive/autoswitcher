#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.63])
AC_INIT([Scim Panelcontroller], [1.0.1], [tim_armstrong@sil.org])
AC_CONFIG_SRCDIR([src/scim_panelcontrol_client.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])


MAJOR_VERSION=1
MINOR_VERSION=0
MAJOR_MINOR=$MAJOR_VERSION.$MINOR_VERSION
REQUIRED_SCIM_VERSION=1.4.9

LIBRARY_NAME=scimpanelcontroller-$MAJOR_MINOR

AC_SUBST(MAJOR_VERSION)
AC_SUBST(MINOR_VERSION)
AC_SUBST(MAJOR_MINOR)
AC_SUBST(REQUIRED_SCIM_VERSION)
AC_SUBST(LIBRARY_NAME)

AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
LT_INIT

# Checks for libraries.
PKG_CHECK_MODULES(SCIM, scim >= $REQUIRED_SCIM_VERSION)
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
	scimpanelcontroller.pc
	])
AC_OUTPUT
