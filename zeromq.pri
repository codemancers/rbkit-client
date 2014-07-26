# Include this file into your project to build and link to zeromq

HEADERS += \
    $$OUT_PWD/../zeromq/include/zmq.h \
    $$OUT_PWD/../zeromq/include/zmq_utils.h

INCLUDEPATH += $$OUT_PWD/../zeromq/include

LIBS += $$OUT_PWD/../zeromq/lib/libzmq.a
