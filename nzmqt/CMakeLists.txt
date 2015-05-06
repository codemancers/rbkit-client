cmake_minimum_required(VERSION 3.2.2)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/local)

# find packages
find_package(Qt5Core)

include_directories(
    include
    3rdparty/cppzmq
    ${CMAKE_SOURCE_DIR}/zeromq-4.0.4/include)


add_library(nzmqt
    src/nzmqt/nzmqt.cpp

    include/nzmqt/global.hpp
    include/nzmqt/nzmqt.hpp
    include/nzmqt/impl.hpp)


target_link_libraries(nzmqt Qt5::Core)
