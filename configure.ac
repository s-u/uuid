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

AC_CHECK_HEADERS([netinet/in.h stdlib.h sys/file.h \
 sys/ioctl.h sys/socket.h sys/sockio.h sys/syscall.h sys/time.h sys/un.h \
 unistd.h])
AC_CHECK_HEADERS([net/if.h net/if_dl.h], [], [], 
[#if HAVE_NETINET_IN_H 
#include <netinet/in.h> 
#endif 
])

AC_CHECK_FUNCS([jrand48])
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

## NOTE: the current util-linux which has taken over libuuid also uses
## check for HAVE_TLS (thread local storage).
## However, the AX_CHECK_TLS test in util-linux/m4/tls.m4 is taken from
## GCC under GPLv2 and we don't want it to infect our license, so we
## don't check for it. In the current code the only difference when
## HAVE_TLS is defined is that it adds __thread to static declarations.
## Given that R uses libuuid only on the main thread this should have
## no real impact.

AC_CONFIG_FILES([src/Makevars])
AC_OUTPUT
