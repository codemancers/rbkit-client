QT       += core gui webkitwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = RbkitClient

TEMPLATE = app

SOURCES += main.cpp


# Include rbkit lib via pri file
include($$RC_ROOT_SOURCE_DIR/rbkit/rbkit.pri)

# Include msgpack via pri file
include($$RC_ROOT_SOURCE_DIR/msgpack.pri)

# Include nzmqt via pri file
include($$RC_ROOT_SOURCE_DIR/nzmqt/nzmqt.pri)

# Include zeromq via pri file
include($$RC_ROOT_SOURCE_DIR/zeromq.pri)

# Include zeromq via pri file
include($$RC_ROOT_SOURCE_DIR/web/web.pri)
