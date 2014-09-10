QT += core gui webkitwidgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport sql

VERSION = 0.0.0.pre
TARGET = rbkit
TEMPLATE = lib

CONFIG += staticlib
CONFIG += c++11

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
    heapdumpform.cpp \
    stringutil.cpp \
    memoryview.cpp \
    appstate.cpp \
    sqlconnectionpool.cpp \
    heapitem.cpp \
    heapdatamodel.cpp \
    sortobjectproxymodel.cpp

HEADERS +=  \
    subscriber.h \
    rbkitmainwindow.h \
    askhost.h \
    zmqsockets.h \
    rbevents.h \
    objectdetail.h \
    objectstore.h \
    jsbridge.h \
    heapdumpform.h \
    stringutil.h \
    memoryview.h \
    appstate.h \
    sqlconnectionpool.h \
    heapitem.h \
    heapdatamodel.h \
    sortobjectproxymodel.h


# Include msgpack via pri file
include($$RC_ROOT_SOURCE_DIR/common.pri)

FORMS += \
    rbkitmainwindow.ui \
    askhost.ui \
    heapdumpform.ui \
    memoryview.ui
