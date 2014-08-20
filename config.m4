dnl $Id$
dnl config.m4 for extension posixclocks

PHP_ARG_WITH(posixclocks, for posixclocks support,
[  --with-posixclocks             Include POSIX clock_gettime support])

if test "$PHP_POSIXCLOCKS" != "no"; then
  AC_CHECK_HEADERS(time.h)

  PHP_NEW_EXTENSION(posixclocks, posixclocks.c, $ext_shared)
fi
