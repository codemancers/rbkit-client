QT       += core gui webkitwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

VERSION = 0.0.0.pre
TARGET = RbkitClient
TEMPLATE = app

SOURCES += \
            main.cpp \
    subscriber.cpp \
    rbkitmainwindow.cpp \
    askhost.cpp

HEADERS +=  \
    subscriber.h \
    rbkitmainwindow.h \
    askhost.h

msgpack.target = $$OUT_PWD/msgpack/lib/libmsgpack.a
msgpack.commands = mkdir -p $$OUT_PWD/msgpack; cd $$OUT_PWD/msgpack; $$PWD/msgpack-c/configure --prefix=$$OUT_PWD/msgpack; make; make install

zeromq.target = $$OUT_PWD/zeromq/lib/libzmq.a
zeromq.commands = mkdir -p $$OUT_PWD/zeromq; cd $$OUT_PWD/zeromq; $$PWD/zeromq-4.0.4/configure --prefix=$$OUT_PWD/zeromq; make; make install

COFFEESCRIPT_FILES += $$PWD/web/*.coffee
CoffeeMaker.input = COFFEESCRIPT_FILES
CoffeeMaker.targetdir = $$PWD/web
CoffeeMaker.output = $$CoffeeScriptCompiler.targetdir\${QMAKE_FILE_IN_BASE}${QMAKE_FILE_EXT}
CoffeeMaker.commands =  coffee --bare --compile --output $$PWD/web $$PWD/web

CoffeeMaker.CONFIG += no_link_no_clean
CoffeeMaker.variable_out = PRE_TARGETDEPS

QMAKE_EXTRA_COMPILERS += CoffeeMaker


QMAKE_EXTRA_TARGETS += msgpack zeromq charts
PRE_TARGETDEPS +=$$OUT_PWD/msgpack/lib/libmsgpack.a $$OUT_PWD/zeromq/lib/libzmq.a


# Include MsgPack
LIBS += $$OUT_PWD/msgpack/lib/libmsgpack.a
INCLUDEPATH += $$OUT_PWD/msgpack/include
DEPENDPATH += $$OUT_PWD/msgpack/include
HEADERS += $$OUT_PWD/msgpack/include/msgpack.hpp

# Include ZeroMQ
LIBS += $$OUT_PWD/zeromq/lib/libzmq.a
INCLUDEPATH += $$OUT_PWD/zeromq/include
INCLUDEPATH += $$PWD/zeromq/include
DEPENDPATH += $$PWD/zeromq/include
HEADERS += $$PWD/zeromq/include/zmq.hpp

FORMS += \
    rbkitmainwindow.ui \
    askhost.ui

RESOURCES += \
    RbkitClient.qrc

