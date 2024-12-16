cmake_minimum_required(VERSION 3.16)

project(qt_02 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Core Network Sql)

add_executable(qt_02
  main.cpp
)
target_link_libraries(qt_02 Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(qt_02 Qt6::Core Qt6::Network Qt6::Sql)

include(GNUInstallDirs)
install(TARGETS qt_02
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
