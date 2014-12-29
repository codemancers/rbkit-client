QT       += core gui webkitwidgets sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport sql

TARGET = RbkitClient

TEMPLATE = app

SOURCES += main.cpp

CONFIG += c++11

# Include rbkit related include and lib paths
INCLUDEPATH += $$RC_ROOT_SOURCE_DIR/rbkit-lib
LIBS += $$RC_ROOT_BUILD_DIR/rbkit-lib/librbkit.a

# Include msgpack via pri file
include($$RC_ROOT_SOURCE_DIR/common.pri)


# Include rbkit-charts via pri file
include($$RC_ROOT_SOURCE_DIR/rbkit-charts/rbcharts.pri)

RESOURCES += \
    appicon.qrc
