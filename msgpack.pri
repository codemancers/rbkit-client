# Include this file into your project to build and link to zeromq

HEADERS += $$OUT_PWD/../zeromq/include/msgpack.hpp

INCLUDEPATH += $$OUT_PWD/../msgpack/include

LIBS += $$OUT_PWD/../msgpack/lib/libmsgpack.a
