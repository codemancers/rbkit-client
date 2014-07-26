QT += core gui webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

VERSION = 0.0.0.pre
TARGET = rbkit
TEMPLATE = lib
RBKIT_ROOT = $$PWD/..

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
include(../msgpack.pri)

# Include nzmqt via pri file
include(../nzmqt/nzmqt.pri)

# Include zeromq via pri file
include(../zeromq.pri)

FORMS += \
    rbkitmainwindow.ui \
    askhost.ui

RESOURCES += \
    rbkit.qrc
