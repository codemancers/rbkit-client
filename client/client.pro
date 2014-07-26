QT       += core gui webkitwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = RbkitClient

TEMPLATE = app

SOURCES += main.cpp


# Include rbkit lib via pri file
include(../rbkit/rbkit.pri)

# Include msgpack via pri file
include(../msgpack.pri)

# Include nzmqt via pri file
# include(../nzmqt/nzmqt.pri)

# Include zeromq via pri file
include(../zeromq.pri)
