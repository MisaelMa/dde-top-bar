cmake_minimum_required(VERSION 3.5)

project(dde-top-bar LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BIN_NAME "dde-top-bar")
find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(dde-top-bar
  main.cpp
  panel/mainpanel.cpp
  panel/mainpanel.h
  utils/global.cpp
  utils/global.h
)

target_link_libraries(dde-top-bar PRIVATE Qt5::Widgets)


find_package(PkgConfig REQUIRED)
find_package(DtkWidget REQUIRED)
find_package(Qt5 COMPONENTS Core Gui DBus Svg X11Extras Concurrent REQUIRED)

pkg_check_modules(PROCPS REQUIRED libprocps)
pkg_check_modules(XCB_LIBS REQUIRED xcb-ewmh xcb xcb-image xcb-composite xtst xcb-icccm dbusmenu-qt5)
pkg_check_modules(XCB_EWMH REQUIRED xcb-ewmh x11 xext)
pkg_check_modules(DFrameworkDBus REQUIRED dframeworkdbus)
pkg_check_modules(DDE-Network-Utils REQUIRED dde-network-utils)
pkg_check_modules(QGSettings REQUIRED gsettings-qt)

target_include_directories(${BIN_NAME} PUBLIC
                                        ${DtkWidget_INCLUDE_DIRS}
                                        ${XCB_EWMH_INCLUDE_DIRS}
                                        ${DFrameworkDBus_INCLUDE_DIRS}
                                        ${Qt5Gui_PRIVATE_INCLUDE_DIRS}
                                        ${Qt5DBus_INCLUDE_DIRS}
                                        ${QGSettings_INCLUDE_DIRS}
                                        ${DDE-Network-Utils_INCLUDE_DIRS}
                                        ${XCB_LIBS_INCLUDE_DIRS}
                                        ${PROJECT_BINARY_DIR}
)

target_link_libraries(${BIN_NAME} PRIVATE
        ${DFrameworkDBus_LIBRARIES}
        ${QGSettings_LIBRARIES}
        ${DDE-Network-Utils_LIBRARIES}
        ${XCB_LIBS_LIBRARIES}
        ${XCB_EWMH_LIBRARIES}
        ${DtkWidget_LIBRARIES}
        Qt5::Core
        Qt5::Gui
        Qt5::Svg
        Qt5::DBus
        Qt5::X11Extras
        Qt5::Concurrent
        ${PROCPS_LIBRARIES}
)
