# Process this file with autoconf to produce a configure script.
AC_INIT([uuid],[0.1],[Simon.Urbanek@r-project.org])
AC_CONFIG_SRCDIR([src/R.c])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_AUX_DIR([tools])

# find R home and set CC/CFLAGS
: ${R_HOME=`R RHOME`}
if test -z "${R_HOME}"; then
  echo "could not determine R_HOME"
  exit 1
fi
RBIN="${R_HOME}/bin/R"
CC=`"${RBIN}" CMD config CC`;
## since R 3.6 we can't use CPP but NEWS recommends $CC -E
CPP="${CC} -E"
CFLAGS=`"${RBIN}" CMD config CFLAGS`
CPPFLAGS=`"${RBIN}" CMD config CPPFLAGS`
if test -n "${PKG_CPPFLAGS}"; then
   CPPFLAGS="${CPPFLAGS} ${PKG_CPPFLAGS}"
fi

AC_PREFIX_DEFAULT([/usr])
AC_CANONICAL_HOST
AC_PROG_CC

## this may look a bit weird, but the path specs need recursive evaluation
## which we don't get with AC_DEFINE_UNQUOTED so we need to get to the
## real path by following the crumbs
AS_CASE([$prefix],
[NONE | /usr],
[AS_CASE([$localstatedir], [NONE | '${prefix}/var' | "${prefix}"/var ], [localstatedir=/var])]
)
AC_SUBST([localstatedir])
AS_IF([test x"$runstatedir" = x], [runstatedir='${localstatedir}/run'])
AS_CASE([$localstatedir:$runstatedir], [NONE:'${localstatedir}/run' | /var:'${localstatedir}/run' | NONE:'/run' ], [runstatedir=/run])
AC_SUBST([runstatedir])

support_uuidd=auto
AC_ARG_ENABLE([uuidd],
  AS_HELP_STRING([--disable-uuidd], [do not build support for using the uuid daemon]),
  [support_uuidd=${enable_uuidd}], []
)
AC_MSG_CHECKING([whether uuidd daemon should be supported])
## by default we build uuidd support only on Linux
if test x${support_uuidd} = xauto; then
  case "${host_os}" in
    linux*) support_uuidd=yes ;;
  esac
fi
if test x${support_uuidd} = xyes; then
    AC_DEFINE([HAVE_UUIDD], [1], [Define to 1 if you want to use uuid daemon.])
    AC_DEFINE_UNQUOTED([_PATH_RUNSTATEDIR], ["$runstatedir"], [Define path to the run state directory.])
    AC_MSG_RESULT([yes])
    AC_MSG_NOTICE([Expecting daemon socket in runstatedir: $runstatedir under uuidd/request])
else
    AC_MSG_RESULT([no])
    support_uuidd=no
fi

LIBS="${PKG_LIBS}"

AC_CHECK_HEADERS([grp.h langinfo.h locale.h netinet/in.h stdlib.h sys/file.h \
 sys/ioctl.h sys/socket.h sys/sockio.h sys/syscall.h sys/sendfile.h sys/signalfd.h\
 sys/time.h sys/un.h unistd.h])
AC_CHECK_HEADERS([net/if.h net/if_dl.h], [], [], 
[#if HAVE_NETINET_IN_H 
#include <netinet/in.h> 
#endif 
])

AC_CHECK_FUNCS([ \
  dirfd \
  err \
  errx \
  getexecname \
  getrandom \
  jrand48 \
  llseek \
  lchown \
  mempcpy \
  mkostemp \
  nanosleep \
  sendfile \
  srandom \
  strnchr \
  strndup \
  strnlen \
  usleep \
  warn \
  warnx
])
AC_CHECK_FUNCS([flock],
[#if HAVE_SYS_FILE_H
#include <sys/file.h>
#endif
])
AC_CHECK_MEMBER(struct sockaddr.sa_len,
                AC_DEFINE_UNQUOTED(HAVE_SA_LEN,1,[Define if struct sockaddr contains sa_len]),,
        [#include <sys/types.h>
         #include <sys/socket.h>])

AC_CHECK_HEADERS([dlfcn.h])
AC_SEARCH_LIBS(dlsym, dl, [AC_DEFINE(HAS_DLSYM, 1, [defined if the system supports dlsym])])

## note the libuuid only uses __thread so that's the only one to test for
have_tls=no
AC_MSG_CHECKING([for thread local storage (TLS) support])
AC_COMPILE_IFELSE(
	[AC_LANG_PROGRAM([],[static __thread int x;])],
       	[have_tls=yes])
AC_MSG_RESULT([$have_tls])
if test x$have_tls = xyes; then
	AC_DEFINE(HAVE_TLS, 1, [Thread local storage is supported via __thread])
fi

AC_CONFIG_FILES([src/Makevars])
AC_OUTPUT
