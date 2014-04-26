#-------------------------------------------------
#
# Project created by QtCreator 2014-04-24T20:12:56
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MsgpackExample
TEMPLATE = app

SOURCES +=  client.cpp\
            main.cpp

HEADERS +=  client.h

LIBS     += $$PWD/msgpack-c/lib/libmsgpack.a

INCLUDEPATH += $$PWD/msgpack-c/include
DEPENDPATH += $$PWD/msgpack-c/include
HEADERS += $$PWD/msgpack-c/include/msgpack.hpp


