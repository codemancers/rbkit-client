cmake_minimum_required(VERSION 3.2.2)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

# find packages
find_package(Qt5WebKitWidgets)
find_package(Qt5PrintSupport)
find_package(Qt5Sql)


include_directories(
    ${CMAKE_SOURCE_DIR}/msgpack-c/src/msgpack
    ${CMAKE_SOURCE_DIR}/msgpack-c/src
    ${CMAKE_SOURCE_DIR}/nzmqt/include
    ${CMAKE_SOURCE_DIR}/nzmqt/3rdparty/cppzmq
    ${CMAKE_SOURCE_DIR}/zeromq-4.0.4/include)


QT5_WRAP_UI(RBKIT_UI_HEADERS
    ui/rbkitmainwindow.ui
    ui/askhost.ui
    ui/heapdumpform.ui
    ui/memoryview.ui
    ui/comparesnapshotform.ui
    ui/aboutdialog.ui
    ui/parentviewform.ui)


QT5_ADD_RESOURCES(RBKIT_UI_RESOURCES
    tool_icons.qrc)



add_library(rbkit
    subscriber.cpp
    rbcommands.cpp
    zmqsockets.cpp
    mpparser.cpp
    rbevents.cpp
    rbeventparser.cpp
    stringutil.cpp
    sqlconnectionpool.cpp
    model/appstate.cpp
    model/heapdatamodel.cpp
    model/jsbridge.cpp
    model/objectaggregator.cpp
    model/objectdetail.cpp
    model/objectstore.cpp
    model/snapshotstate.cpp
    model/sortobjectproxymodel.cpp
    ui/askhost.cpp
    ui/comparesnapshotform.cpp
    ui/diffviewform.cpp
    ui/heapdumpform.cpp
    ui/memoryview.cpp
    ui/actiontoolbar.cpp
    ui/aboutdialog.cpp
    model/parentobject.cpp
    ui/parentviewform.cpp
    model/heap_item_types/baseheapitem.cpp
    model/heap_item_types/heapitem.cpp
    model/heap_item_types/leafitem.cpp
    ui/processdetail.cpp
    ui/appmainwindow.cpp
    ui/centralwidget.cpp
    ui/ribbontoolbar.cpp
    cpu/cpunode.cpp
    cpu/cpuprof.cpp
    cpu/cpustorage.cpp

    subscriber.h
    zmqsockets.h
    mpparser.h
    rbevents.h
    rbeventparser.h
    rbcommands.h
    stringutil.h
    sqlconnectionpool.h
    model/appstate.h
    model/heapdatamodel.h
    model/jsbridge.h
    model/objectaggregator.h
    model/objectdetail.h
    model/objectstore.h
    model/sortobjectproxymodel.h
    ui/askhost.h
    ui/comparesnapshotform.h
    ui/diffviewform.h
    ui/heapdumpform.h
    ui/memoryview.h
    ui/actiontoolbar.h
    ui/aboutdialog.h
    model/parentobject.h
    ui/parentviewform.h
    model/heap_item_types/baseheapitem.h
    model/heap_item_types/heapitem.h
    model/heap_item_types/leafitem.h
    model/snapshotstate.h
    ui/processdetail.h
    ui/appmainwindow.h
    ui/centralwidget.h
    ui/ribbontoolbar.h
    layoututil.hpp
    common.h

    cpu/cpuprof.h
    cpu/cpustorage.h
    cpu/cpunode.h
    cpu/cpumapping.h

    ${RBKIT_UI_HEADERS}
    ${RBKIT_UI_RESOURCES})


set_property(TARGET rbkit PROPERTY CXX_STANDARD 11)


target_link_libraries(rbkit Qt5::WebKitWidgets)
target_link_libraries(rbkit Qt5::Sql)
