QT       += core gui webkitwidgets sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport sql

TARGET = RbkitClient

TEMPLATE = app

SOURCES += main.cpp

CONFIG += c++11

# Include msgpack via pri file
include($$RC_ROOT_SOURCE_DIR/common.pri)

# Include rbkit-charts via pri file
include($$RC_ROOT_SOURCE_DIR/rbkit-charts/rbcharts.pri)

RESOURCES += \
    appicon.qrc
