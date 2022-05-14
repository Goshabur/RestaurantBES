cmake_minimum_required(VERSION 3.20)
project(RestaurantBES)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets Quick Qml REQUIRED)

include_directories(images)

add_executable(QtGosha
        src/main.cpp
        src/qml.qrc
        src/images.qrc
        )

target_link_libraries(QtGosha crypto ssl pthread)
target_link_libraries(QtGosha Qt5::Widgets Qt5::Quick Qt5::Qml)