include(../rbkit/rbkit.pri)

QT       += core gui webkitwidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = RbkitClient

TEMPLATE = app

SOURCES += main.cpp

LIBS += $$OUT_PWD/../rbkit/librbkit.a
LIBS += $$OUT_PWD/../rbkit/msgpack/lib/libmsgpack.a
LIBS += $$OUT_PWD/../rbkit/zeromq/lib/libzmq.a
