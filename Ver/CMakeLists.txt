cmake_minimum_required(VERSION 3.20)
project(RestaurantBES)

set(CMAKE_CXX_STANDARD 17)

include_directories(./ServerExample/server/include)

add_executable(Server
        ServerExample/server/source/user.cpp
        ServerExample/server/source/server.cpp
        ServerExample/server/source/server_main.cpp
        ServerExample/server/source/handlers.cpp
        ServerExample/server/source/session.cpp)
target_link_directories(Server PRIVATE $ENV{HOME}/folly_installed/installed/folly/lib)

target_include_directories(Server PRIVATE $ENV{HOME}/restbed/source)
target_include_directories(Server PRIVATE $ENV{HOME}/folly_installed/installed/folly/include)
target_include_directories(Server PRIVATE $ENV{HOME}/folly_installed/installed/glog-2Q8xht8wcxfXuQlWzaHdvPGfSj9lC1BK8dozBXhSo6o/include)

target_link_libraries(Server restbed crypto ssl pthread gflags folly dl fmt)



include(FetchContent)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets Quick Qml REQUIRED)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
FetchContent_MakeAvailable(json)

include_directories(QtClient/include)

add_executable(QtClient
        QtClient/src/main.cpp
        QtClient/src/qml.qrc
        QtClient/src/images.qrc

        QtClient/include/MenuItem.h
        QtClient/include/MenuList.h
        QtClient/src/MenuList.cpp

        QtClient/include/MenuModel.h
        QtClient/src/MenuModel.cpp

        QtClient/include/CartList.h
        QtClient/src/CartList.cpp

        QtClient/include/Client.h
        QtClient/src/Client.cpp

        QtClient/src/jsonParser.cpp
        )

target_link_directories(QtClient PRIVATE $ENV{HOME}/folly_installed/installed/folly/lib)

target_include_directories(QtClient PRIVATE $ENV{HOME}/folly_installed/installed/folly/include)
target_include_directories(QtClient PRIVATE $ENV{HOME}/folly_installed/installed/glog-2Q8xht8wcxfXuQlWzaHdvPGfSj9lC1BK8dozBXhSo6o/include)

target_link_libraries(QtClient crypto ssl pthread folly dl fmt gflags)
target_link_libraries(QtClient Qt5::Widgets Qt5::Quick Qt5::Qml)
target_link_libraries(QtClient nlohmann_json::nlohmann_json)


