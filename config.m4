dnl $Id$
dnl config.m4 for extension posixclocks

PHP_ARG_ENABLE(posixclocks, whether to enable posixclocks support,
[  --enable-posixclocks    Enable POSIX clock_gettime() support])

if test ! -f "$srcdir/posixclocks_ver.h"; then
  if test -d "$srcdir/.git"; then
	version=`git log -1 --pretty=format:'git#%h (%ci)'`
  else
    version=Unknown
  fi
  `cat <<EOF > posixclocks_ver.h
#define PHP_PSXCLK_VERSION "$version"
EOF`
fi

if test "$PHP_PSXCLK" != "no"; then
  AC_CHECK_HEADER(time.h, ,[echo "Error: The header <time.h> was not found"; exit 1])
  
  dnl  AC_CHECK_FUNC(clock_gettime, ,[echo "Error: The clock_gettime(2) function was not found"; exit 1])
  dnl  AC_CHECK_FUNC(clock_getres, ,[echo "Error: The clock_getres(2) function was not found"; exit 1])

  AC_CHECK_FUNC(clock_gettime, [],
    [ 
      dnl On Linux, try syscall wrapper first
      if test $(uname) = Linux; then
        AC_MSG_CHECKING(for clock_gettime syscall)
          AC_LINK_IFELSE(
            [
              AC_LANG_PROGRAM(
                [#include <syscall.h> #include <time.h>],
		        [struct timespec ts; int status = syscall (SYS_clock_gettime, CLOCK_REALTIME, &ts)]
              )
            ],
		    [
              ac_have_clock_syscall=1
              AC_DEFINE(HAVE_CLOCK_SYSCALL, 1, "use syscall interface for clock_gettime")
              AC_MSG_RESULT(yes)
            ],
		    [AC_MSG_RESULT(no)]
          )
      fi
      if test -z "$LIBEV_M4_AVOID_LIBRT" && test -z "$ac_have_clock_syscall"; then
        AC_CHECK_LIB(rt, clock_gettime) 
        unset ac_cv_func_clock_gettime
        AC_CHECK_FUNCS(clock_gettime)
      fi
    ]
  )

  AC_CHECK_FUNC(clock_getres, [],
    [ 
      dnl On Linux, try syscall wrapper first
      if test $(uname) = Linux; then
        AC_MSG_CHECKING(for clock_getres syscall)
          AC_LINK_IFELSE(
            [
              AC_LANG_PROGRAM(
                [#include <syscall.h> #include <time.h>],
		        [struct timespec ts; int status = syscall (SYS_clock_getres, CLOCK_REALTIME, &ts)]
              )
            ],
		    [
              ac_have_clock_syscall=1
              AC_DEFINE(HAVE_CLOCK_SYSCALL, 1, "use syscall interface for clock_getres")
              AC_MSG_RESULT(yes)
            ],
		    [AC_MSG_RESULT(no)]
          )
      fi
      if test -z "$LIBEV_M4_AVOID_LIBRT" && test -z "$ac_have_clock_syscall"; then
        AC_CHECK_LIB(rt, clock_getres) 
        unset ac_cv_func_clock_getres
        AC_CHECK_FUNCS(clock_getres)
      fi
    ]
  )

  PHP_NEW_EXTENSION(posixclocks, posixclocks.c, $ext_shared)
fi
