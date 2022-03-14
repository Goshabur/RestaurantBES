cmake_minimum_required(VERSION 3.20)
project(RestaurantBES)

set(CMAKE_CXX_STANDARD 17)

include_directories(./server/include)

add_executable(Server
        server/source/message.cpp
        server/source/chat.cpp
        server/source/user.cpp
        server/source/server.cpp
        server/source/server_main.cpp
        )
target_include_directories(Server PRIVATE $ENV{HOME}/restbed/source)
target_link_libraries(Server restbed crypto ssl pthread gflags)

add_executable(Client
        client/client.cpp
        )
target_link_libraries(Client crypto ssl pthread gflags)
