dnl $Id$
dnl config.m4 for extension posixclocks

PHP_ARG_WITH(posixclocks, for posixclocks support,
[  --with-posixclocks             Include POSIX clock_gettime support])

if test "$PHP_POSIXCLOCKS" != "no"; then
  dnl Write more examples of tests here...

  # --with-posixclocks -> check with-path
  SEARCH_PATH="/usr/local /usr"
  SEARCH_FOR="/include/time.h"
  if test -r $PHP_POSIXCLOCKS/$SEARCH_FOR; then # path given as parameter
    POSIXCLOCKS_DIR=$PHP_POSIXCLOCKS
  else # search default path list
    AC_MSG_CHECKING([for posixclocks files in default path])
    for i in $SEARCH_PATH ; do
      if test -r $i/$SEARCH_FOR; then
        POSIXCLOCKS_DIR=$i
        AC_MSG_RESULT(found in $i)
      fi
    done
  fi

  dnl if test -z "$POSIXCLOCKS_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the posixclocks distribution])
  dnl fi

  dnl # --with-posixclocks -> add include path
  dnl PHP_ADD_INCLUDE($POSIXCLOCKS_DIR/include)

  dnl # --with-posixclocks -> check for lib and symbol presence
  dnl LIBNAME=posixclocks # you may want to change this
  dnl LIBSYMBOL=posixclocks # you most likely want to change this

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $POSIXCLOCKS_DIR/lib, POSIXCLOCKS_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_POSIXCLOCKSLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong posixclocks lib version or lib not found])
  dnl ],[
  dnl   -L$POSIXCLOCKS_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(POSIXCLOCKS_SHARED_LIBADD)

  PHP_NEW_EXTENSION(posixclocks, posixclocks.c, $ext_shared)
fi
