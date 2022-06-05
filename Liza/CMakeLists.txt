include(FetchContent)

cmake_minimum_required(VERSION 3.16)
project(RestaurantBES)

include_directories(/usr/local/include ${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR})

include_directories(../Ver/Server/include)
include_directories(./include)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(Boost_USE_MULTITHREADED ON)

find_package(PostgreSQL REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(CURL)

if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    add_definitions(-DHAVE_CURL)
endif ()

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
        libpqxx
        GIT_REPOSITORY https://github.com/jtv/libpqxx.git
        GIT_TAG a6b1d60e74c1427c8ac2324b85cd4a0dc2068332
)

set(PQXX_LIBRARIES pqxx_static)
set(FOLLY_DIRECTORY $ENV{HOME}/folly_installed_partial/installed/)
set(FMT fmt)
set(GLOG glog)
set(DOUBLE_CONVERSION double-conversion)

set(FOLLY_INCLUDE ${FOLLY_DIRECTORY}/folly/include
        ${FOLLY_DIRECTORY}/${GLOG}/include
        ${FOLLY_DIRECTORY}/${FMT}/include
        ${FOLLY_DIRECTORY}/${DOUBLE_CONVERSION}/include)

set(FOLLY_LIB ${FOLLY_DIRECTORY}/folly/lib
        ${FOLLY_DIRECTORY}/${GLOG}/lib
        ${FOLLY_DIRECTORY}/${FMT}/lib
        ${FOLLY_DIRECTORY}/${DOUBLE_CONVERSION}/lib)

FetchContent_MakeAvailable(libpqxx)

add_library(Server STATIC
        ../Ver/Server/src/user.cpp
        ../Ver/Server/src/server.cpp
        ../Ver/Server/src/session.cpp
        )

target_include_directories(Server PRIVATE $ENV{HOME}/restbed/source)
target_link_directories(Server PRIVATE $ENV{HOME}/restbed/build)

target_link_directories(Server PRIVATE ${FOLLY_LIB})
target_include_directories(Server PRIVATE ${FOLLY_INCLUDE})

target_link_libraries(Server restbed crypto ssl pthread gflags folly dl fmt)

add_subdirectory(tgbot-cpp)

add_executable(RestaurantBES
        src/admin.cpp
        src/client.cpp
        src/main.cpp
        src/handlers.cpp
        src/cart.cpp
        src/order.cpp
        src/server.cpp
        src/tgBot.cpp
        )

target_link_directories(RestaurantBES PRIVATE ${FOLLY_LIB})
target_include_directories(RestaurantBES PRIVATE ${FOLLY_INCLUDE})

target_include_directories(RestaurantBES PRIVATE $ENV{HOME}/restbed/source)
target_link_directories(RestaurantBES PRIVATE $ENV{HOME}/restbed/build)

target_link_libraries(RestaurantBES Server TgBot)
target_link_libraries(RestaurantBES restbed crypto ssl pthread gflags folly double-conversion dl fmt glog nlohmann_json::nlohmann_json)
target_link_libraries(RestaurantBES ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES} ${PQXX_LIBRARIES})