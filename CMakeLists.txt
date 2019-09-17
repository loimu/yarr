project(yarr)
cmake_minimum_required(VERSION 2.8.9)

set(CMAKE_AUTOMOC ON)

set(yarr_SRCS src/main.cpp
              src/mainwindow.cpp
              src/board.cpp
              src/boardmodel.cpp
              src/painter.cpp
              src/player.cpp
              src/reversiplayer.cpp
              src/settings.cpp
              src/solver.cpp
              src/tests.cpp
              src/aboutdialog.cpp)
set(yarr_FRMS src/mainwindow.ui src/settings.ui src/aboutdialog.ui)
set(yarr_RSRCS yarr.qrc)

find_package(Qt5Core QUIET)
find_package(Qt5Widgets)
find_package(Qt5Network)
find_package(Qt5Test)
include_directories(${Qt5Core_INCLUDE_DIRS}
                    ${Qt5Widgets_INCLUDE_DIRS}
                    ${Qt5Network_INCLUDE_DIRS}
                    ${CMAKE_CURRENT_BINARY_DIR})
qt5_wrap_ui(yarr_FORMS_HEADERS ${yarr_FRMS})
qt5_add_resources(yarr_RESOURCES_RCC ${yarr_RSRCS})
add_executable(yarr ${yarr_SRCS} ${yarr_HEADERS_MOC} ${yarr_FORMS_HEADERS} ${yarr_RESOURCES_RCC})
target_link_libraries(yarr Qt5::Core Qt5::Widgets Qt5::Network Qt5::Test)

install(TARGETS yarr RUNTIME DESTINATION bin)
