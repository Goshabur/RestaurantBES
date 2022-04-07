cmake_minimum_required(VERSION 3.20)
project(RestaurantBES)

set(CMAKE_CXX_STANDARD 17)

include_directories(./server/include)

add_executable(Server
        server/source/user.cpp
        server/source/server.cpp
        server/source/server_main.cpp
        server/source/handlers.cpp)
target_link_directories(Server PRIVATE $ENV{HOME}/folly_installed/installed/folly/lib)

target_include_directories(Server PRIVATE $ENV{HOME}/restbed/source)

target_include_directories(Server PRIVATE $ENV{HOME}/folly_installed/installed/folly/include)
target_include_directories(Server PRIVATE $ENV{HOME}/folly_installed/installed/glog-2Q8xht8wcxfXuQlWzaHdvPGfSj9lC1BK8dozBXhSo6o/include)

target_link_libraries(Server restbed crypto ssl pthread gflags folly dl fmt)

add_executable(Client
        client/client.cpp
        )
target_link_libraries(Client crypto ssl pthread gflags)
