QT += core gui webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

VERSION = 0.0.0.pre
TARGET = rbkit
TEMPLATE = lib

CONFIG += staticlib

SOURCES += \
    subscriber.cpp \
    rbkitmainwindow.cpp \
    askhost.cpp \
    rbcommands.cpp \
    zmqsockets.cpp \
    rbevents.cpp \
    objectdetail.cpp \
    objectstore.cpp \
    jsbridge.cpp \
    heapdumpform.cpp

HEADERS +=  \
    subscriber.h \
    rbkitmainwindow.h \
    askhost.h \
    zmqsockets.h \
    rbevents.h \
    objectdetail.h \
    objectstore.h \
    jsbridge.h \
    heapdumpform.h


# Include msgpack via pri file
include($$RC_ROOT_SOURCE_DIR/common.pri)

FORMS += \
    rbkitmainwindow.ui \
    askhost.ui \
    heapdumpform.ui
