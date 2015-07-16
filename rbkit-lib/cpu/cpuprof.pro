QT       += core gui webkitwidgets testlib sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport sql

TARGET = cpuprof

CONFIG -= app_bundle
CONFIG += console
CONFIG += c++11

# Include rbkit related include and lib paths
INCLUDEPATH += $$RC_ROOT_SOURCE_DIR/rbkit-lib
LIBS += $$RC_ROOT_BUILD_DIR/rbkit-lib/librbkit.a

# Include everything via one common pri file
include($$RC_ROOT_SOURCE_DIR/common.pri)


SOURCES += \
    main.cpp \
    storage.cpp \
    node.cpp \
    cpuprof.cpp

HEADERS += \
    storage.h \
    node.h \
    mapping.h \
    cpuprof.h
