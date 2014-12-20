# Include msgpack related include and lib paths
INCLUDEPATH += $$RC_ROOT_BUILD_DIR/msgpack/include
LIBS += $$RC_ROOT_BUILD_DIR/msgpack/lib/libmsgpack.a

# Include zeromq related include and lib paths
INCLUDEPATH += $$RC_ROOT_BUILD_DIR/zeromq/include
LIBS += $$RC_ROOT_BUILD_DIR/zeromq/lib/libzmq.a

# Include sqlite related include and lib paths
INCLUDEPATH += $$RC_ROOT_BUILD_DIR/sqlite/include
LIBS += $$RC_ROOT_BUILD_DIR/sqlite/lib/libsqlite3.a

# Include nzmqt via pri file
include($$RC_ROOT_SOURCE_DIR/nzmqt/nzmqt.pri)
