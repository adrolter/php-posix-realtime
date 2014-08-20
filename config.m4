dnl $Id$
dnl config.m4 for extension posixclocks

PHP_ARG_ENABLE(posixclocks, whether to enable posixclocks support,
[  --enable-posixclocks    Enable POSIX clock_gettime() support])

if test "$PHP_POSIXCLOCKS" != "no"; then
  AC_CHECK_HEADER(time.h, ,[echo "Error: The header <time.h> was not found"; exit 1])
  AC_CHECK_FUNC(clock_gettime, ,[echo "Error: The clock_gettime(2) function was not found"; exit 1])
  AC_CHECK_FUNC(clock_getres, ,[echo "Error: The clock_getres(2) function was not found"; exit 1])

  PHP_NEW_EXTENSION(posixclocks, posixclocks.c, $ext_shared)
fi
