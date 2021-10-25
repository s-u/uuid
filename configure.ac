# Process this file with autoconf to produce a configure script.
AC_INIT([uuid],[0.1],[Simon.Urbanek@r-project.org])
AC_CONFIG_SRCDIR([src/R.c])
AC_CONFIG_HEADERS([src/config.h])

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

AC_PROG_CC

LIBS="${PKG_LIBS}"

AC_CHECK_HEADERS([grp.h langinfo.h locale.h netinet/in.h stdlib.h sys/file.h \
 sys/ioctl.h sys/socket.h sys/sockio.h sys/syscall.h sys/sendfile.h \
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

## copy LinkingTo: header files
mkdir -p inst/include
cp src/*uuid.h inst/include/

AC_CONFIG_FILES([src/Makevars])
AC_OUTPUT
