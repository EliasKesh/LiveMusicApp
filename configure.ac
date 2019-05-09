#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([LiveMusicApp], [1.3.0], [elias@kesh.com])
# AM_INIT_AUTOMAKE
AM_INIT_AUTOMAKE([subdir-objects])
AM_PROG_LIBTOOL
AC_CONFIG_MACRO_DIRS([m4])

# AC_CONFIG_SRCDIR([src/Connections.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lX11':
AC_CHECK_LIB([X11], [main])
# FIXME: Replace `main' with a function in `-lXmu':
AC_CHECK_LIB([Xmu], [main])
# FIXME: Replace `main' with a function in `-lm':
AC_CHECK_LIB([m], [main])
# FIXME: Replace `main' with a function in `-lrt':
AC_CHECK_LIB([rt], [main])
# FIXME: Replace `main' with a function in `-lxmltok':
AC_CHECK_LIB([xmltok], [main])
AC_CHECK_LIB([pthread], [main])

PKG_CHECK_MODULES(GTK3, [gtk+-3.0])
PKG_CHECK_MODULES(ALSA, [alsa])
PKG_CHECK_MODULES(GLADEUI, [gladeui-2.0])
PKG_CHECK_MODULES(WEBKIT2GTK, [webkit2gtk-4.0])
PKG_CHECK_MODULES(JACK, [jack])
PKG_CHECK_MODULES(LIBLO, [liblo])

# Checks for header files.
AC_PATH_X
AC_CHECK_HEADERS([fcntl.h locale.h malloc.h stdlib.h string.h sys/ioctl.h sys/time.h termios.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_PID_T
AC_C_RESTRICT
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_ALLOCA
AC_FUNC_ERROR_AT_LINE
AC_FUNC_FORK
AC_FUNC_MALLOC
AC_CHECK_FUNCS([bzero gettimeofday memset mkfifo pow realpath setlocale strchr strerror strrchr strstr])

# AC_CONFIG_FILES([ChartFixer/Makefile
#                 Makefile
#                 src/Makefile])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
