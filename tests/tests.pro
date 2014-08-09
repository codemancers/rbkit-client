QT       += core gui webkitwidgets testlib
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = rctests

CONFIG -= app_bundle
CONFIG += console

SOURCES += \
    testobjectstore.cpp \
    testrbevents.cpp \
    testjsbridge.cpp \
    main.cpp

# Include everything via one common pri file
include($$RC_ROOT_SOURCE_DIR/common.pri)

# install
target.path = $$OUT_PWD/test
INSTALLS += target

HEADERS += \
    AutoTest.h \
    testobjectstore.h \
    testrbevents.h \
    testjsbridge.h

RESOURCES += \
    tests.qrc
