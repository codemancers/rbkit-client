MAKEFILE = Makefile.sqlite

Makefile.target   = Makefile
Makefile.commands = $$PWD/configure --prefix=$$RC_ROOT_BUILD_DIR/sqlite --disable-tcl

all.commands = make && make install
all.depends  = Makefile
all.CONFIG   = phony

TARGET = $$RC_ROOT_BUILD_DIR/sqlite/lib
TEMPLATE = lib

QMAKE_DISTCLEAN += Makefile
QMAKE_EXTRA_TARGETS += Makefile all
