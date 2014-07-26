QT       += core gui webkitwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = RbkitClient

TEMPLATE = app

SOURCES += main.cpp

# Include msgpack via pri file
include($$RC_ROOT_SOURCE_DIR/common.pri)

# Include zeromq via pri file
include($$RC_ROOT_SOURCE_DIR/web/web.pri)
