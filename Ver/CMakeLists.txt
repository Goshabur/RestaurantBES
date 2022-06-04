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

find_package(Qt5 COMPONENTS Widgets Quick Qml REQUIRED)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
FetchContent_MakeAvailable(json)

include_directories(QtClient/include)

include_directories(QtClient/include)

add_library(QtMenu STATIC
        QtClient/include/MenuItem.h
        QtClient/include/MenuList.h
        QtClient/src/MenuList.cpp)
target_link_libraries(QtMenu
        Qt5::Widgets)

add_library(QtCart STATIC
        QtClient/include/CartList.h
        QtClient/src/CartList.cpp)
target_link_libraries(QtCart
        Qt5::Widgets)
target_link_directories(QtCart PRIVATE ${FOLLY_DIRECTORY}/folly/lib)
target_link_directories(QtCart PRIVATE ${FOLLY_DIRECTORY}/${FMT}/lib)

target_include_directories(QtCart PRIVATE ${FOLLY_DIRECTORY}/folly/include)
target_include_directories(QtCart PRIVATE ${FOLLY_DIRECTORY}/${GLOG}/include)
target_include_directories(QtCart PRIVATE ${FOLLY_DIRECTORY}/${FMT}/include)
target_include_directories(QtCart PRIVATE ${FOLLY_DIRECTORY}/${DOUBLE_CONVERSIONS}/include)
target_include_directories(QtCart PRIVATE $ENV{HOME}/restbed/restbed/source)

target_link_libraries(QtCart folly dl fmt gflags double-conversion glog)

add_library(QtOrder
        QtClient/include/OrderList.h
        QtClient/src/OrderList.cpp)
target_link_libraries(QtOrder
        Qt5::Widgets)

add_library(JsonParser STATIC
        QtClient/src/jsonParser.cpp
        )
target_link_libraries(JsonParser
        nlohmann_json::nlohmann_json
        Qt5::Widgets
        QtMenu
        QtCart)
target_link_directories(JsonParser PRIVATE ${FOLLY_DIRECTORY}/installed/folly/lib)
target_include_directories(JsonParser PRIVATE ${FOLLY_DIRECTORY}/folly/include)
target_include_directories(JsonParser PRIVATE ${FOLLY_DIRECTORY}/${GLOG}/include)
target_link_libraries(JsonParser folly dl fmt)

add_executable(QtClient
        QtClient/src/main.cpp
        QtClient/src/qml.qrc

        QtClient/include/Client.h
        QtClient/src/Client.cpp

        QtClient/include/MenuModel.h
        QtClient/src/MenuModel.cpp

        QtClient/include/OrderModel.h
        QtClient/src/OrderModel.cpp
        QtClient/include/Order.h
        QtClient/src/Order.cpp)

target_link_directories(QtClient PRIVATE ${FOLLY_DIRECTORY}/folly/lib)
target_link_directories(QtClient PRIVATE ${FOLLY_DIRECTORY}/${FMT}/lib)

target_include_directories(QtClient PRIVATE ${FOLLY_DIRECTORY}/folly/include)
target_include_directories(QtClient PRIVATE ${FOLLY_DIRECTORY}/${GLOG}/include)
target_include_directories(QtClient PRIVATE ${FOLLY_DIRECTORY}/${FMT}/include)
target_include_directories(QtClient PRIVATE ${FOLLY_DIRECTORY}/${DOUBLE_CONVERSIONS}/include)
target_include_directories(QtClient PRIVATE $ENV{HOME}/restbed/restbed/source)

target_link_libraries(QtClient crypto ssl pthread folly dl fmt gflags)
target_link_libraries(QtClient Qt5::Widgets Qt5::Quick)
target_link_libraries(QtClient QtMenu QtCart QtOrder JsonParser)