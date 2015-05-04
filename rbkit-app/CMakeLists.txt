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
    ${CMAKE_SOURCE_DIR}/rbkit-lib)


QT5_ADD_RESOURCES(RBKIT_APP_RESOURCES
    appicon.qrc
    ${CMAKE_SOURCE_DIR}/rbkit-charts/rbcharts.qrc)


add_executable(RbkitClient
    main.cpp

    ${RBKIT_APP_RESOURCES})


set_property(TARGET RbkitClient PROPERTY CXX_STANDARD 11)


target_link_libraries(RbkitClient Qt5::WebKitWidgets)
target_link_libraries(RbkitClient Qt5::Sql)
target_link_libraries(RbkitClient rbkit)
target_link_libraries(RbkitClient msgpack)
target_link_libraries(RbkitClient libzmq)
target_link_libraries(RbkitClient nzmqt)
