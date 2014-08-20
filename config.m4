dnl $Id$
dnl config.m4 for extension posixclocks

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(posixclocks, for posixclocks support,
dnl Make sure that the comment is aligned:
dnl [  --with-posixclocks             Include posixclocks support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(posixclocks, whether to enable posixclocks support,
dnl Make sure that the comment is aligned:
[  --enable-posixclocks           Enable posixclocks support])

if test "$PHP_POSIXCLOCKS" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-posixclocks -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/posixclocks.h"  # you most likely want to change this
  dnl if test -r $PHP_POSIXCLOCKS/$SEARCH_FOR; then # path given as parameter
  dnl   POSIXCLOCKS_DIR=$PHP_POSIXCLOCKS
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for posixclocks files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       POSIXCLOCKS_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
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
