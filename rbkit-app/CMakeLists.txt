cmake_minimum_required(VERSION 3.2.2)


set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)


if (WIN32)
    set(QT_USE_QTMAIN TRUE)
endif (WIN32)


if (APPLE)
  # set how it shows up in the Info.plist file
  set(MACOSX_BUNDLE_ICON_FILE rbkit.icns)
  # set where in the bundle to put the icns file
  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/rbkit.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
endif (APPLE)


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


add_executable(RbkitClient MACOSX_BUNDLE WIN32
    main.cpp

    ${RBKIT_APP_RESOURCES})


set_property(TARGET RbkitClient PROPERTY CXX_STANDARD 11)


target_link_libraries(RbkitClient Qt5::WebKitWidgets)
target_link_libraries(RbkitClient Qt5::Sql)
target_link_libraries(RbkitClient rbkit)
target_link_libraries(RbkitClient msgpack)
target_link_libraries(RbkitClient libzmq)
target_link_libraries(RbkitClient nzmqt)


set(plugin_dest_dir bin)
set(qtconf_dest_dir bin)
set(APPS "\${CMAKE_INSTALL_PREFIX}/bin/RbkitClient")

if (APPLE)
  set(plugin_dest_dir QtTest.app/Contents/MacOS)
  set(qtconf_dest_dir QtTest.app/Contents/Resources)
  set(APPS "\${CMAKE_INSTALL_PREFIX}/RbkitClient.app")
endif (APPLE)

if (WIN32)
  set(APPS "\${CMAKE_INSTALL_PREFIX}/bin/RbkitClient.exe")
endif (WIN32)

install(TARGETS RbkitClient
    BUNDLE DESTINATION . COMPONENT Runtime
    RUNTIME DESTINATION bin COMPONENT Runtime
    )

set(DIRS ${QT_LIBRARY_DIRS})

set(CPACK_BINARY_DRAGNDROP ON)
include(CPack)
