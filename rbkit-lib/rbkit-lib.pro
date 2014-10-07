QT += core gui webkitwidgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport sql

VERSION = 0.2.0.pre
TARGET = rbkit
TEMPLATE = lib

CONFIG += staticlib
CONFIG += c++11

SOURCES += \
    subscriber.cpp \
    rbcommands.cpp \
    zmqsockets.cpp \
    rbevents.cpp \
    stringutil.cpp \
    sqlconnectionpool.cpp\
    model/appstate.cpp \
    model/heapdatamodel.cpp \
    model/heapitem.cpp \
    model/jsbridge.cpp \
    model/objectaggregator.cpp \
    model/objectdetail.cpp \
    model/objectstore.cpp \
    model/snapshotstate.cpp \
    model/sortobjectproxymodel.cpp \
    ui/askhost.cpp \
    ui/comapresnapshotform.cpp \
    ui/diffviewform.cpp \
    ui/heapdumpform.cpp \
    ui/memoryview.cpp \
    ui/rbkitmainwindow.cpp \
    ui/actiontoolbar.cpp \
    ui/aboutdialog.cpp

HEADERS +=  \
    subscriber.h \
    zmqsockets.h \
    rbevents.h \
    rbcommands.h \
    stringutil.h \
    sqlconnectionpool.h \
    model/appstate.h \
    model/heapdatamodel.h \
    model/heapitem.h \
    model/jsbridge.h \
    model/objectaggregator.h \
    model/objectdetail.h \
    model/objectstore.h \
    model/snapshotstate.h \
    model/sortobjectproxymodel.h \
    ui/askhost.h \
    ui/comapresnapshotform.h \
    ui/diffviewform.h \
    ui/heapdumpform.h \
    ui/memoryview.h \
    ui/rbkitmainwindow.h \
    ui/actiontoolbar.h \
    ui/aboutdialog.h


# Include msgpack via pri file
include($$RC_ROOT_SOURCE_DIR/common.pri)

FORMS += \
    ui/rbkitmainwindow.ui \
    ui/askhost.ui \
    ui/heapdumpform.ui \
    ui/memoryview.ui \
    ui/comapresnapshotform.ui \
    ui/aboutdialog.ui

RESOURCES += \
    tool_icons.qrc
