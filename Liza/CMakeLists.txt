include(FetchContent)

cmake_minimum_required(VERSION 3.20)
project(RestaurantBES)

include_directories(./server/include)
include_directories(/usr/local/include ${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")
set(Boost_USE_MULTITHREADED ON)

find_package(PostgreSQL REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(CURL)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
        libpqxx
        GIT_REPOSITORY https://github.com/jtv/libpqxx.git
        GIT_TAG a6b1d60e74c1427c8ac2324b85cd4a0dc2068332
)
set(PQXX_LIBRARIES pqxx_static)

FetchContent_MakeAvailable(libpqxx)

add_executable(RestaurantBES src/server.cpp
        src/admin.cpp src/client.cpp
        server/src/user.cpp
        server/src/server_main.cpp
        server/src/server.cpp
        server/src/handlers.cpp
        server/src/session.cpp
        )

target_link_directories(RestaurantBES PRIVATE $ENV{HOME}/folly_installed/installed/folly/lib)
target_include_directories(RestaurantBES PRIVATE $ENV{HOME}/folly_installed/installed/folly/include)
target_link_directories(RestaurantBES PRIVATE $ENV{HOME}/folly_installed/installed/fmt-G-S79T7s0y6Nnej83wZiZVdv4C7yFWpiMPbiszI6qrE/lib)
target_link_directories(RestaurantBES PRIVATE $ENV{HOME}/folly_installed/installed/glog-hlx3KJNmWcAtpfcbv-FURS7ajZk1KR1M1LeNXUanJck/lib)
target_link_directories(RestaurantBES PRIVATE $ENV{HOME}/folly_installed/installed/double-conversion-SeMNpgJWTLYvzAlvJj42BLnPB2NwSiyEOWeS43W3eJI/lib)

target_include_directories(RestaurantBES PRIVATE $ENV{HOME}/folly_installed/installed/glog-hlx3KJNmWcAtpfcbv-FURS7ajZk1KR1M1LeNXUanJck/include)
target_include_directories(RestaurantBES PRIVATE $ENV{HOME}/folly_installed/installed/fmt-G-S79T7s0y6Nnej83wZiZVdv4C7yFWpiMPbiszI6qrE/include)
target_include_directories(RestaurantBES PRIVATE $ENV{HOME}/folly_installed/installed/double-conversion-SeMNpgJWTLYvzAlvJj42BLnPB2NwSiyEOWeS43W3eJI/include)
target_include_directories(RestaurantBES PRIVATE $ENV{HOME}/restbed/source)
target_include_directories(RestaurantBES PRIVATE $ENV{HOME}/restbed/restbed/source)

target_link_libraries(RestaurantBES restbed crypto ssl pthread gflags folly double-conversion dl fmt glog)
target_link_libraries(RestaurantBES "${PQXX_LIBRARIES}" nlohmann_json::nlohmann_json)

if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    add_definitions(-DHAVE_CURL)
endif ()

add_subdirectory(tgbot-cpp)
add_executable(TelegramBot Telegram_Bot/tgBot.cpp src/admin.cpp)

target_link_directories(TelegramBot PRIVATE $ENV{HOME}/folly_installed/installed/folly/lib)
target_link_directories(TelegramBot PRIVATE $ENV{HOME}/folly_installed/installed/fmt/lib)
target_link_directories(TelegramBot PRIVATE $ENV{HOME}/folly_installed/installed/double-conversion/lib)
target_link_directories(TelegramBot PRIVATE $ENV{HOME}/restbed/restbed/build)

target_include_directories(TelegramBot PRIVATE $ENV{HOME}/restbed/restbed/source)
target_include_directories(TelegramBot PRIVATE $ENV{HOME}/folly_installed/installed/folly/include)
target_include_directories(TelegramBot PRIVATE $ENV{HOME}/folly_installed/installed/glog/include)
target_include_directories(TelegramBot PRIVATE $ENV{HOME}/folly_installed/installed/fmt/include)
target_include_directories(TelegramBot PRIVATE $ENV{HOME}/folly_installed/installed/double-conversion/include)

target_link_libraries(TelegramBot "${PQXX_LIBRARIES}" nlohmann_json::nlohmann_json)
target_link_libraries(TelegramBot TgBot ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES})

include(FetchContent)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Widgets Quick Qml REQUIRED)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
FetchContent_MakeAvailable(json)

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
target_link_directories(QtCart PRIVATE $ENV{HOME}/folly_installed/installed/folly/lib)
target_link_directories(QtCart PRIVATE $ENV{HOME}/folly_installed/installed/fmt-G-S79T7s0y6Nnej83wZiZVdv4C7yFWpiMPbiszI6qrE/lib)

target_include_directories(QtCart PRIVATE $ENV{HOME}/folly_installed/installed/folly/include)
target_include_directories(QtCart PRIVATE $ENV{HOME}/folly_installed/installed/glog-hlx3KJNmWcAtpfcbv-FURS7ajZk1KR1M1LeNXUanJck/include)
target_include_directories(QtCart PRIVATE $ENV{HOME}/folly_installed/installed/fmt-G-S79T7s0y6Nnej83wZiZVdv4C7yFWpiMPbiszI6qrE/include)
target_include_directories(QtCart PRIVATE $ENV{HOME}/folly_installed/installed/double-conversion-SeMNpgJWTLYvzAlvJj42BLnPB2NwSiyEOWeS43W3eJI/include)
target_include_directories(QtCart PRIVATE $ENV{HOME}/restbed/restbed/source)

target_link_libraries(QtCart folly dl fmt gflags)

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
target_link_directories(JsonParser PRIVATE $ENV{HOME}/folly_installed/installed/folly/lib)
target_include_directories(JsonParser PRIVATE $ENV{HOME}/folly_installed/installed/folly/include)
target_include_directories(JsonParser PRIVATE $ENV{HOME}/folly_installed/installed/glog-2Q8xht8wcxfXuQlWzaHdvPGfSj9lC1BK8dozBXhSo6o/include)
target_link_libraries(JsonParser folly dl fmt)

add_executable(QtClient
        QtClient/src/main.cpp
        QtClient/src/qml.qrc
        QtClient/src/images.qrc

        QtClient/include/Client.h
        QtClient/src/Client.cpp

        QtClient/include/MenuModel.h
        QtClient/src/MenuModel.cpp

        QtClient/include/OrderModel.h
        QtClient/src/OrderModel.cpp
        QtClient/include/Order.h QtClient/src/Order.cpp)

target_link_directories(QtClient PRIVATE $ENV{HOME}/folly_installed/installed/folly/lib)
target_link_directories(QtClient PRIVATE $ENV{HOME}/folly_installed/installed/fmt-G-S79T7s0y6Nnej83wZiZVdv4C7yFWpiMPbiszI6qrE/lib)

target_include_directories(QtClient PRIVATE $ENV{HOME}/folly_installed/installed/folly/include)
target_include_directories(QtClient PRIVATE $ENV{HOME}/folly_installed/installed/glog-hlx3KJNmWcAtpfcbv-FURS7ajZk1KR1M1LeNXUanJck/include)
target_include_directories(QtClient PRIVATE $ENV{HOME}/folly_installed/installed/fmt-G-S79T7s0y6Nnej83wZiZVdv4C7yFWpiMPbiszI6qrE/include)
target_include_directories(QtClient PRIVATE $ENV{HOME}/folly_installed/installed/double-conversion-SeMNpgJWTLYvzAlvJj42BLnPB2NwSiyEOWeS43W3eJI/include)
target_include_directories(QtClient PRIVATE $ENV{HOME}/restbed/restbed/source)

target_link_libraries(QtClient crypto ssl pthread folly dl fmt gflags)
target_link_libraries(QtClient Qt5::Widgets Qt5::Quick)
target_link_libraries(QtClient QtMenu QtCart QtOrder JsonParser)