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

# Include MsgPack
LIBS     += $$PWD/msgpack-c/lib/libmsgpack.a
INCLUDEPATH += $$PWD/msgpack-c/include
DEPENDPATH += $$PWD/msgpack-c/include
HEADERS += $$PWD/msgpack-c/include/msgpack.hpp

# Include ZeroMQ
LIBS     += $$PWD/zeromq-4.0.4/lib/libzmq.a
INCLUDEPATH += $$PWD/zeromq-4.0.4/include
DEPENDPATH += $$PWD/zeromq-4.0.4/include
HEADERS += $$PWD/zeromq-4.0.4/include/zmq.hpp

FORMS += \
    dialog.ui
