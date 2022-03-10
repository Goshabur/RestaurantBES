cmake_minimum_required(VERSION 3.20)
project(RestaurantBES)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(test-chat
        test-chat.cpp)
target_include_directories(test-chat PRIVATE $ENV{HOME}/restbed/source)
target_link_libraries(test-chat pthread)

add_executable(Server
        server.cpp)
target_include_directories(Server PRIVATE $ENV{HOME}/restbed/source)
target_link_libraries(Server restbed crypto ssl pthread)

add_executable(Client
        client.cpp)
target_link_libraries(Client crypto ssl pthread)
