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

msgpack.target = $$PWD/msgpack-c/lib/libmsgpack.a
msgpack.commands = cd $$PWD/msgpack-c; ./bootstrap ; ./configure --prefix=$$PWD/msgpack-c; make; make install

zeromq.target = $$PWD/zeromq/lib/libzmq.a
zeromq.commands = cd $$PWD/zeromq-4.0.4; ./configure --prefix=$$PWD/zeromq; make; make install

COFFEESCRIPT_FILES += $$PWD/web/*.coffee
CoffeeMaker.input = COFFEESCRIPT_FILES
CoffeeMaker.targetdir = $$PWD/web
CoffeeMaker.output = $$CoffeeScriptCompiler.targetdir\${QMAKE_FILE_IN_BASE}${QMAKE_FILE_EXT}
CoffeeMaker.commands =  coffee --bare --compile --output $$PWD/web $$PWD/web

CoffeeMaker.CONFIG += no_link_no_clean
CoffeeMaker.variable_out = PRE_TARGETDEPS

QMAKE_EXTRA_COMPILERS += CoffeeMaker


QMAKE_EXTRA_TARGETS += msgpack zeromq charts
PRE_TARGETDEPS +=$$PWD/msgpack-c/lib/libmsgpack.a $$PWD/zeromq/lib/libzmq.a


# Include MsgPack
LIBS += $$PWD/msgpack-c/lib/libmsgpack.a
INCLUDEPATH += $$PWD/msgpack-c/include
DEPENDPATH += $$PWD/msgpack-c/include
HEADERS += $$PWD/msgpack-c/include/msgpack.hpp

# Include ZeroMQ
LIBS += $$PWD/zeromq/lib/libzmq.a
INCLUDEPATH += $$PWD/zeromq/include
DEPENDPATH += $$PWD/zeromq/include
HEADERS += $$PWD/zeromq/include/zmq.hpp

FORMS += \
    rbkitmainwindow.ui \
    askhost.ui

RESOURCES += \
    RbkitClient.qrc

