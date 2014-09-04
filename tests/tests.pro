QT       += core gui webkitwidgets testlib sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport sql

TARGET = rctests

CONFIG -= app_bundle
CONFIG += console
CONFIG += c++11

SOURCES += \
    testobjectstore.cpp \
    testrbevents.cpp \
    testjsbridge.cpp \
    main.cpp \
    testappstate.cpp

# Include everything via one common pri file
include($$RC_ROOT_SOURCE_DIR/common.pri)

# install
target.path = $$OUT_PWD/test
INSTALLS += target

HEADERS += \
    AutoTest.h \
    testobjectstore.h \
    testrbevents.h \
    testjsbridge.h \
    testappstate.h

RESOURCES += \
    tests.qrc
