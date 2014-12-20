msgpack.subdir   = msgpack-c
msgpack.makefile = Makefile.msgpack

zeromq.subdir   = zeromq-4.0.4
zeromq.makefile = Makefile.zeromq

sqlite.subdir   = sqlite3
sqlite.makefile = Makefile.sqlite

TEMPLATE = subdirs
SUBDIRS =     \
    msgpack   \
    zeromq    \
    sqlite    \
    rbkit-lib \
    rbkit-app \
    tests

CONFIG += ordered
CONFIG += c++11
CONFIG += static
