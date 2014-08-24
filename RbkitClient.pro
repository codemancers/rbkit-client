msgpack.subdir   = msgpack-c
msgpack.makefile = Makefile.msgpack

zeromq.subdir   = zeromq-4.0.4
zeromq.makefile = Makefile.zeromq

TEMPLATE = subdirs
SUBDIRS =     \
    msgpack   \
    zeromq    \
    rbkit-lib \
    rbkit-app \
    tests

CONFIG += ordered
CONFIG += c++11
