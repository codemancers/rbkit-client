QT += core gui webkitwidgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport sql

VERSION = 0.0.0.pre
TARGET = rbkit
TEMPLATE = lib

CONFIG += staticlib
CONFIG += c++11

SOURCES += ui/*
SOURCES += model/*

SOURCES += \
    subscriber.cpp \
    rbcommands.cpp \
    zmqsockets.cpp \
    rbevents.cpp \
    stringutil.cpp \
    sqlconnectionpool.cpp

HEADERS += ui/*
HEADERS += model/*

HEADERS +=  \
    subscriber.h \
    zmqsockets.h \
    rbevents.h \
    rbcommands.h \
    stringutil.h \
    sqlconnectionpool.h


# Include msgpack via pri file
include($$RC_ROOT_SOURCE_DIR/common.pri)

FORMS += \
    ui/rbkitmainwindow.ui \
    ui/askhost.ui \
    ui/heapdumpform.ui \
    ui/memoryview.ui \
    ui/comapresnapshotform.ui

RESOURCES += \
    tool_icons.qrc
