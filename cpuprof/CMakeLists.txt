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


#add_executable(cpuprof
#    main.cpp
#    storage.cpp
#    node.cpp
#    cpuprof.cpp

#    cpustorage.h
#    node.h
#    mapping.h
#    cpuprof.h)


#set_property(TARGET cpuprof PROPERTY CXX_STANDARD 11)


#target_link_libraries(cpuprof Qt5::WebKitWidgets)
#target_link_libraries(cpuprof Qt5::Sql)
#target_link_libraries(cpuprof Qt5::Test)
#target_link_libraries(cpuprof rbkit)
#target_link_libraries(cpuprof msgpack)
#target_link_libraries(cpuprof libzmq)
#target_link_libraries(cpuprof nzmqt)
#add_executable(cpuprof
#    main.cpp
#    storage.cpp
#    node.cpp
#    cpuprof.cpp

#    storage.h
#    node.h
#    mapping.h
#    cpuprof.h)


set_property(TARGET cpuprof PROPERTY CXX_STANDARD 11)


target_link_libraries(cpuprof Qt5::WebKitWidgets)
target_link_libraries(cpuprof Qt5::Sql)
target_link_libraries(cpuprof Qt5::Test)
target_link_libraries(cpuprof rbkit)
target_link_libraries(cpuprof msgpack)
target_link_libraries(cpuprof libzmq)
target_link_libraries(cpuprof nzmqt)
