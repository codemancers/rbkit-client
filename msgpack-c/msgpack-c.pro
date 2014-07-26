RBKIT_BUILD_ROOT = $$OUT_PWD/..
RBKIT_SRC_ROOT = $$PWD/..

MAKEFILE = Makefile.msgpack

Makefile.target   = Makefile
Makefile.commands = $$PWD/configure --prefix=$${RBKIT_BUILD_ROOT}/msgpack

all.commands = make && make install
all.depends  = Makefile
all.CONFIG   = phony

TARGET   = $${RBKIT_BUILD_ROOT}/msgpack/lib/libmsgpack.a
TEMPLATE = lib

QMAKE_DISTCLEAN += Makefile
QMAKE_EXTRA_TARGETS += Makefile all
