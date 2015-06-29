# Process this file with autoconf to produce a configure script.
AC_INIT(uuid, 0.1, Simon.Urbanek@r-project.org)
AC_CONFIG_SRCDIR([src/R.c])
AC_CONFIG_HEADER([src/config.h])

# find R home and set CC/CFLAGS
: ${R_HOME=`R RHOME`}
if test -z "${R_HOME}"; then
  echo "could not determine R_HOME"
  exit 1
fi
RBIN="${R_HOME}/bin/R"
CC=`"${RBIN}" CMD config CC`;
CPP=`"${RBIN}" CMD config CPP`;
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

AC_CONFIG_FILES([src/Makevars])
AC_OUTPUT
