QT += core gui webkitwidgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport sql

VERSION = 0.2.0.pre
TARGET = rbkit
TEMPLATE = lib

CONFIG += staticlib
CONFIG += c++11

# Include msgpack via pri file
include($$RC_ROOT_SOURCE_DIR/common.pri)

SOURCES += \
    subscriber.cpp \
    rbcommands.cpp \
    zmqsockets.cpp \
    mpparser.cpp \
    rbevents.cpp \
    rbeventparser.cpp \
    stringutil.cpp \
    sqlconnectionpool.cpp\
    model/appstate.cpp \
    model/heapdatamodel.cpp \
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
    ui/aboutdialog.cpp \
    model/parentobject.cpp \
    ui/parentviewform.cpp \
    model/heap_item_types/baseheapitem.cpp \
    model/heap_item_types/heapitem.cpp \
    model/heap_item_types/leafitem.cpp \
    ui/processdetail.cpp

HEADERS +=  \
    subscriber.h \
    zmqsockets.h \
    mpparser.h \
    rbevents.h \
    rbeventparser.h \
    rbcommands.h \
    stringutil.h \
    sqlconnectionpool.h \
    model/appstate.h \
    model/heapdatamodel.h \
    model/jsbridge.h \
    model/objectaggregator.h \
    model/objectdetail.h \
    model/objectstore.h \
    model/sortobjectproxymodel.h \
    ui/askhost.h \
    ui/comapresnapshotform.h \
    ui/diffviewform.h \
    ui/heapdumpform.h \
    ui/memoryview.h \
    ui/rbkitmainwindow.h \
    ui/actiontoolbar.h \
    ui/aboutdialog.h \
    model/parentobject.h \
    ui/parentviewform.h \
    model/heap_item_types/baseheapitem.h \
    model/heap_item_types/heapitem.h \
    model/heap_item_types/leafitem.h \
    model/snapshotstate.h \
    ui/processdetail.h

FORMS += \
    ui/rbkitmainwindow.ui \
    ui/askhost.ui \
    ui/heapdumpform.ui \
    ui/memoryview.ui \
    ui/comapresnapshotform.ui \
    ui/aboutdialog.ui \
    ui/parentviewform.ui

RESOURCES += \
    tool_icons.qrc
