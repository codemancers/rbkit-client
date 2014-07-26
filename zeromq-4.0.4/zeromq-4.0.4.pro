MAKEFILE = Makefile.zeromq

Makefile.target   = Makefile
Makefile.commands = $$PWD/configure --prefix=$$RC_ROOT_BUILD_DIR/zeromq

all.commands = make && make install
all.depends  = Makefile
all.CONFIG   = phony

TARGET = $$RC_ROOT_BUILD_DIR/zeromq/lib/zeromq.a
TEMPLATE = lib

QMAKE_DISTCLEAN += Makefile
QMAKE_EXTRA_TARGETS += Makefile all
