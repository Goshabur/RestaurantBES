cmake_minimum_required(VERSION 3.20)
project(RestaurantBES)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(FOLLY_DIRECTORY $ENV{HOME}/folly_installed/installed/)
set(FMT fmt-rsFOHg1lnSE4YEAOp4DXj1C2TkL43wt032fPM5H9wFE)
set(GLOG glog-2Q8xht8wcxfXuQlWzaHdvPGfSj9lC1BK8dozBXhSo6o)
set(DOUBLE_CONVERSIONS double-conversion-yz-tOOI9shRwd_c7D6KsoefZQgMsavQLPYRzthcDoSM)

set(FOLLY_INCLUDE ${FOLLY_DIRECTORY}/folly/include
${FOLLY_DIRECTORY}/${GLOG}/include
${FOLLY_DIRECTORY}/${FMT}/include
${FOLLY_DIRECTORY}/${DOUBLE_CONVERSIONS}/include)

set(FOLLY_LIB ${FOLLY_DIRECTORY}/folly/lib
${FOLLY_DIRECTORY}/${GLOG}/lib
${FOLLY_DIRECTORY}/${FMT}/lib
${FOLLY_DIRECTORY}/${DOUBLE_CONVERSIONS}/lib)

find_package(Qt5 COMPONENTS Widgets Quick Qml REQUIRED)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
FetchContent_MakeAvailable(json)

include_directories(QtClient/include)

add_executable(QtClient
        QtClient/src/main.cpp
        QtClient/src/qml.qrc

        QtClient/include/MenuItem.h
        QtClient/include/MenuList.h
        QtClient/src/MenuList.cpp

        QtClient/include/CartList.h
        QtClient/src/CartList.cpp

        QtClient/include/OrderList.h
        QtClient/src/OrderList.cpp

        QtClient/src/jsonParser.cpp

        QtClient/include/Client.h
        QtClient/src/Client.cpp

        QtClient/include/MenuModel.h
        QtClient/src/MenuModel.cpp

        QtClient/include/OrderModel.h
        QtClient/src/OrderModel.cpp
        QtClient/include/Order.h
        QtClient/src/Order.cpp)

target_link_directories(QtClient PRIVATE ${FOLLY_LIB})
target_include_directories(QtClient PRIVATE ${FOLLY_INCLUDE})

target_link_libraries(QtClient crypto ssl pthread folly dl fmt gflags)
target_link_libraries(QtClient Qt5::Widgets Qt5::Quick nlohmann_json::nlohmann_json)