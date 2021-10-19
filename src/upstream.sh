## update files from upstream (util-linux -> libuuid)
if [ ! -e util-linux ]; then
  git clone --depth=1 https://github.com/karelzak/util-linux.git
fi

for src in \
util-linux/include/all-io.h \
util-linux/include/c.h \
util-linux/libuuid/src/clear.c \
util-linux/libuuid/src/compare.c \
util-linux/libuuid/src/copy.c \
util-linux/libuuid/src/gen_uuid.c \
util-linux/libuuid/src/isnull.c \
util-linux/lib/md5.c \
util-linux/include/md5.h \
util-linux/include/nls.h \
util-linux/libuuid/src/pack.c \
util-linux/libuuid/src/parse.c \
util-linux/lib/randutils.c \
util-linux/include/randutils.h \
util-linux/lib/sha1.c \
util-linux/include/sha1.h \
util-linux/include/strutils.h \
util-linux/libuuid/src/unpack.c \
util-linux/libuuid/src/unparse.c \
util-linux/libuuid/src/uuid.h \
util-linux/libuuid/src/uuidP.h \
util-linux/libuuid/src/uuidd.h \
; do cp -p $src .; done

(cd .. && patch -p1 < src/upstream.patch )
