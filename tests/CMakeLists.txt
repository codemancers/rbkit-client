cmake_minimum_required(VERSION 3.2.2)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)


# find packages
find_package(Qt5WebKitWidgets)
find_package(Qt5PrintSupport)
find_package(Qt5Sql)
find_package(Qt5Test)


include_directories(
    ${CMAKE_SOURCE_DIR}/msgpack-c/src/msgpack
    ${CMAKE_SOURCE_DIR}/msgpack-c/src
    ${CMAKE_SOURCE_DIR}/rbkit-lib)


QT5_ADD_RESOURCES(RBKIT_TESTS_RESOURCES
    tests.qrc)


add_executable(rctests
    testobjectstore.cpp
    testrbevents.cpp
    testjsbridge.cpp
    testobjectdump.cpp
    main.cpp
    testappstate.cpp
    testsnapshotstate.cpp

    AutoTest.h
    testobjectstore.h
    testrbevents.h
    testjsbridge.h
    testobjectdump.h
    testappstate.h
    testsnapshotstate.h

    ${RBKIT_TESTS_RESOURCES})


set_property(TARGET rctests PROPERTY CXX_STANDARD 11)


target_link_libraries(rctests Qt5::WebKitWidgets)
target_link_libraries(rctests Qt5::Sql)
target_link_libraries(rctests Qt5::Test)
target_link_libraries(rctests rbkit)
target_link_libraries(rctests msgpack)
target_link_libraries(rctests libzmq)
target_link_libraries(rctests nzmqt)
