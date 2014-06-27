QT       += core gui webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

VERSION = 0.0.0.pre
TARGET = RbkitClient
TEMPLATE = app

SOURCES +=  client.cpp\
            main.cpp \
    subscriber.cpp

HEADERS +=  client.h \
    subscriber.h

msgpack.target = $$PWD/msgpack-c/lib/libmsgpack.a
msgpack.commands = cd $$PWD/msgpack-c; ./bootstrap ; ./configure --prefix=$$PWD/msgpack-c; make; make install

zeromq.target = $$PWD/zeromq/lib/libzmq.a
zeromq.commands = cd $$PWD/zeromq-4.0.4; ./configure --prefix=$$PWD/zeromq; make; make install
QMAKE_EXTRA_TARGETS += msgpack zeromq
PRE_TARGETDEPS +=$$PWD/msgpack-c/lib/libmsgpack.a $$PWD/zeromq/lib/libzmq.a


# Include MsgPack
LIBS += $$PWD/msgpack-c/lib/libmsgpack.a
INCLUDEPATH += $$PWD/msgpack-c/include
DEPENDPATH += $$PWD/msgpack-c/include
HEADERS += $$PWD/msgpack-c/include/msgpack.hpp

# Include ZeroMQ
LIBS += $$PWD/zeromq/lib/libzmq.a
INCLUDEPATH += $$PWD/zeromq/include
DEPENDPATH += $$PWD/zeromq/include
HEADERS += $$PWD/zeromq/include/zmq.hpp

FORMS += \
    dialog.ui

RESOURCES += \
    RbkitClient.qrc

