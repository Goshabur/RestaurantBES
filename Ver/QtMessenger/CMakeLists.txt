cmake_minimum_required(VERSION 3.20)
project(RestaurantBES)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets Quick Qml REQUIRED)

include_directories(include)

add_executable(QtMessenger
        src/main.cpp
        src/qml.qrc
        src/images.qrc
        include/client.h
        src/client.cpp
        )

target_link_libraries(QtMessenger crypto ssl pthread)
target_link_libraries(QtMessenger Qt5::Widgets Qt5::Quick Qt5::Qml)