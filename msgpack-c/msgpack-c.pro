MAKEFILE = Makefile.msgpack

Makefile.target   = Makefile
Makefile.commands = $$PWD/configure --prefix=$$RC_ROOT_BUILD_DIR/msgpack

all.commands = make && make install
all.depends  = Makefile
all.CONFIG   = phony

TARGET   = $$RC_ROOT_BUILD_DIR/msgpack/lib/libmsgpack.a
TEMPLATE = lib

QMAKE_DISTCLEAN += Makefile
QMAKE_EXTRA_TARGETS += Makefile all
