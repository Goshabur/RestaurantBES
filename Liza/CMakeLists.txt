include(FetchContent)

cmake_minimum_required(VERSION 3.20)
project(RestaurantBES)

include_directories(../Ver/QtClient/include)
include_directories(/usr/local/include ${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR})

include_directories(../Ver/ServerExample/include)
include_directories(./include)

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
set(FOLLY_DIRECTORY $ENV{HOME}/folly_installed/installed/)
set(FMT fmt-G-S79T7s0y6Nnej83wZiZVdv4C7yFWpiMPbiszI6qrE)
set(GLOG glog-2Q8xht8wcxfXuQlWzaHdvPGfSj9lC1BK8dozBXhSo6o)
set(DOUBLE_CONVERSIONS double-conversion-SeMNpgJWTLYvzAlvJj42BLnPB2NwSiyEOWeS43W3eJI)

FetchContent_MakeAvailable(libpqxx)

add_library(Server
        ../Ver/ServerExample/src/user.cpp
        ../Ver/ServerExample/src/server.cpp
        ../Ver/ServerExample/src/session.cpp
        )

target_link_directories(Server PRIVATE ${FOLLY_DIRECTORY}/folly/lib)
target_link_directories(Server PRIVATE ${FOLLY_DIRECTORY}/${FMT}/lib)

target_include_directories(Server PRIVATE $ENV{HOME}/restbed/source)
target_include_directories(Server PRIVATE ${FOLLY_DIRECTORY}/folly/include)
target_include_directories(Server PRIVATE ${FOLLY_DIRECTORY}/${GLOG}/include)
target_include_directories(Server PRIVATE ${FOLLY_DIRECTORY}/${FMT}/include)
target_include_directories(Server PRIVATE ${FOLLY_DIRECTORY}/${DOUBLE_CONVERSIONS}/include)
target_include_directories(Server PRIVATE $ENV{HOME}/restbed/restbed/source)

target_link_libraries(Server restbed crypto ssl pthread gflags folly dl fmt)

add_executable(RestaurantBES
        src/admin.cpp
        src/client.cpp
        src/main.cpp
        src/handlers.cpp
        src/cart.cpp
        src/order.cpp
        src/server.cpp
        )

target_link_directories(RestaurantBES PRIVATE ${FOLLY_DIRECTORY}/folly/lib)
target_link_directories(RestaurantBES PRIVATE ${FOLLY_DIRECTORY}/${FMT}/lib)
target_link_directories(RestaurantBES PRIVATE ${FOLLY_DIRECTORY}/${GLOG}/lib)
target_link_directories(RestaurantBES PRIVATE ${FOLLY_DIRECTORY}/${DOUBLE_CONVERSIONS}/lib)

target_include_directories(RestaurantBES PRIVATE ${FOLLY_DIRECTORY}/folly/include)
target_include_directories(RestaurantBES PRIVATE ${FOLLY_DIRECTORY}/${GLOG}/include)
target_include_directories(RestaurantBES PRIVATE ${FOLLY_DIRECTORY}/${FMT}/include)
target_include_directories(RestaurantBES PRIVATE ${FOLLY_DIRECTORY}/${DOUBLE_CONVERSIONS}/include)
target_include_directories(RestaurantBES PRIVATE $ENV{HOME}/restbed/source)
target_include_directories(RestaurantBES PRIVATE $ENV{HOME}/restbed/restbed/source)

target_link_libraries(RestaurantBES Server restbed crypto ssl pthread gflags folly double-conversion dl fmt glog)
target_link_libraries(RestaurantBES "${PQXX_LIBRARIES}" nlohmann_json::nlohmann_json)

if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    add_definitions(-DHAVE_CURL)
endif ()

add_subdirectory(tgbot-cpp)
add_executable(TelegramBot Telegram_Bot/tgBot.cpp src/admin.cpp src/order.cpp src/cart.cpp src/client.cpp src/server.cpp)

target_link_directories(TelegramBot PRIVATE ${FOLLY_DIRECTORY}/folly/lib)
target_link_directories(TelegramBot PRIVATE ${FOLLY_DIRECTORY}/${GLOG}/lib)
target_link_directories(TelegramBot PRIVATE ${FOLLY_DIRECTORY}/${FMT}/lib)
target_link_directories(TelegramBot PRIVATE ${FOLLY_DIRECTORY}/${DOUBLE_CONVERSIONS}/lib)
target_link_directories(TelegramBot PRIVATE $ENV{HOME}/restbed/restbed/build)

target_include_directories(TelegramBot PRIVATE $ENV{HOME}/restbed/restbed/source)
target_include_directories(TelegramBot PRIVATE $ENV{HOME}/restbed/source)
target_include_directories(TelegramBot PRIVATE ${FOLLY_DIRECTORY}/folly/include)
target_include_directories(TelegramBot PRIVATE ${FOLLY_DIRECTORY}/${GLOG}/include)
target_include_directories(TelegramBot PRIVATE ${FOLLY_DIRECTORY}/${FMT}/include)
target_include_directories(TelegramBot PRIVATE ${FOLLY_DIRECTORY}/${DOUBLE_CONVERSIONS}/include)

target_link_libraries(TelegramBot "${PQXX_LIBRARIES}" nlohmann_json::nlohmann_json restbed crypto ssl pthread gflags folly double-conversion dl fmt glog)
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
        ../Ver/QtClient/include/MenuItem.h
        ../Ver/QtClient/include/MenuList.h
        ../Ver/QtClient/src/MenuList.cpp)
target_link_libraries(QtMenu
        Qt5::Widgets)

add_library(QtCart STATIC
        ../Ver/QtClient/include/CartList.h
        ../Ver/QtClient/src/CartList.cpp)
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
        ../Ver/QtClient/include/OrderList.h
        ../Ver/QtClient/src/OrderList.cpp)
target_link_libraries(QtOrder
        Qt5::Widgets)

add_library(JsonParser STATIC
        ../Ver/QtClient/src/jsonParser.cpp
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
        ../Ver/QtClient/src/main.cpp
        ../Ver/QtClient/src/qml.qrc
        ../Ver/QtClient/src/images.qrc

        ../Ver/QtClient/include/Client.h
        ../Ver/QtClient/src/Client.cpp

        ../Ver/QtClient/include/MenuModel.h
        ../Ver/QtClient/src/MenuModel.cpp

        ../Ver/QtClient/include/OrderModel.h
        ../Ver/QtClient/src/OrderModel.cpp
        ../Ver/QtClient/include/Order.h
        ../Ver/QtClient/src/Order.cpp)

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