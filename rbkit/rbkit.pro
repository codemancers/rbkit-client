QT += core gui webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

VERSION = 0.0.0.pre
TARGET = rbkit
TEMPLATE = lib

CONFIG += staticlib

SOURCES += \
    subscriber.cpp \
    rbkitmainwindow.cpp \
    askhost.cpp

HEADERS +=  \
    subscriber.h \
    rbkitmainwindow.h \
    askhost.h

# Include msgpack via pri file
include($$RC_ROOT_SOURCE_DIR/msgpack.pri)

# Include nzmqt via pri file
include($$RC_ROOT_SOURCE_DIR/nzmqt/nzmqt.pri)

# Include zeromq via pri file
include($$RC_ROOT_SOURCE_DIR/zeromq.pri)


FORMS += \
    rbkitmainwindow.ui \
    askhost.ui
