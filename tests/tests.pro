QT       += core gui webkitwidgets testlib
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = rctests

CONFIG -= app_bundle
CONFIG += console
CONFIG += c++11

SOURCES += \
    testobjectstore.cpp \
    testrbevents.cpp \
    testjsbridge.cpp \
    main.cpp \
    testheaptable.cpp \
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
    testheaptable.h \
    testappstate.h

RESOURCES += \
    tests.qrc
