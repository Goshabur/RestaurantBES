cmake_minimum_required(VERSION 3.20)
project(RestaurantBES)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets Quick Qml REQUIRED)

include_directories(include)

add_executable(QtClient
        src/main.cpp
        src/qml.qrc
        src/images.qrc

        include/MenuList.h
        src/MenuList.cpp

        include/MenuModel.h
        src/MenuModel.cpp

        include/CartList.h
        src/CartList.cpp

        include/MenuItem.h
        include/MenuData.h
        src/MenuData.cpp)

target_link_directories(QtClient PRIVATE $ENV{HOME}/folly_installed/installed/folly/lib)

target_include_directories(QtClient PRIVATE $ENV{HOME}/folly_installed/installed/folly/include)
target_include_directories(QtClient PRIVATE $ENV{HOME}/folly_installed/installed/glog-2Q8xht8wcxfXuQlWzaHdvPGfSj9lC1BK8dozBXhSo6o/include)

target_link_libraries(QtClient crypto ssl pthread folly dl fmt)
target_link_libraries(QtClient Qt5::Widgets Qt5::Quick Qt5::Qml)