dnl $Id$
dnl config.m4 for extension bettertime

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(bettertime, for bettertime support,
dnl Make sure that the comment is aligned:
dnl [  --with-bettertime             Include bettertime support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(bettertime, whether to enable bettertime support,
dnl Make sure that the comment is aligned:
[  --enable-bettertime           Enable bettertime support])

if test "$PHP_BETTERTIME" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-bettertime -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/bettertime.h"  # you most likely want to change this
  dnl if test -r $PHP_BETTERTIME/$SEARCH_FOR; then # path given as parameter
  dnl   BETTERTIME_DIR=$PHP_BETTERTIME
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for bettertime files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       BETTERTIME_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$BETTERTIME_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the bettertime distribution])
  dnl fi

  dnl # --with-bettertime -> add include path
  dnl PHP_ADD_INCLUDE($BETTERTIME_DIR/include)

  dnl # --with-bettertime -> check for lib and symbol presence
  dnl LIBNAME=bettertime # you may want to change this
  dnl LIBSYMBOL=bettertime # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $BETTERTIME_DIR/lib, BETTERTIME_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_BETTERTIMELIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong bettertime lib version or lib not found])
  dnl ],[
  dnl   -L$BETTERTIME_DIR/lib -lm
  dnl ])
  dnl
  dnl PHP_SUBST(BETTERTIME_SHARED_LIBADD)

  PHP_NEW_EXTENSION(bettertime, bettertime.c, $ext_shared)
fi
