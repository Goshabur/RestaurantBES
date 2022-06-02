#pragma once

#include <ctime>
#include <fstream>
#include <iomanip>
#include <restbed>
#include <string>
#include <vector>
//#include "../include/admin.h"
//#include "../include/cart.h"
//#include "../include/client.h"
//#include "../include/order.h"
#include "folly/dynamic.h"
#include "folly/json.h"
#include "handlers.h"
#include "nlohmann/json.hpp"
#include "pqxx/pqxx"
#include "session.h"
#include "user.h"

namespace restbes {

static std::time_t time_now = std::time(nullptr);

enum OrderStatus { CREATED = 0, ACCEPTED, CANCELLED, IN_PROGRESS, COMPLETED };
enum DishStatus { UNAVAILABLE = 0, AVAILABLE };

static std::vector<std::string> orderStatuses = {
    "CREATED", "ACCEPTED", "CANCELLED", "IN_PROGRESS", "COMPLETED"};

static std::map<std::string, int> orderStatusesMap = {{"CREATED", 0},
                                                      {"ACCEPTED", 1},
                                                      {"CANCELLED", 2},
                                                      {"IN_PROGRESS", 3},
                                                      {"COMPLETED", 4}};

static std::vector<std::string> dishStatuses = {"UNAVAILABLE", "AVAILABLE"};

static std::map<std::string, int> dishStatusesMap = {{"UNAVAILABLE", 0},
                                                     {"AVAILABLE", 1}};

static std::ofstream server_request_log("server_request_log.txt",
                                        std::ofstream::app);
static std::ofstream server_error_log("server_error_log.txt",
                                      std::ofstream::app);

static void connectExec(const std::string &sql) {
    pqxx::connection C(
        "dbname= user=postgres password=restbes2022 hostaddr=127.0.0.1 "
        "port=5432");

    pqxx::work W(C);
    W.exec(sql);
    W.commit();

    C.disconnect();
}

static std::string connectGet(const std::string &sql) {
    pqxx::connection C(
        "dbname=testdb user=postgres password=restbes2022 hostaddr=127.0.0.1 "
        "port=5432");

    pqxx::nontransaction N(C);
    pqxx::result result(N.exec(sql));
    C.disconnect();

    return result[0][0].c_str();
}

static pqxx::result connectGet_pqxx_result(const std::string &sql) {
    pqxx::connection C(
        "dbname=testdb user=postgres password=restbes2022 hostaddr=127.0.0.1 "
        "port=5432");

    pqxx::nontransaction N(C);
    pqxx::result result(N.exec(sql));
    C.disconnect();

    return result;
}

static bool check_admin(const std::string &password) {
    try {
        connectGet(
            R"(SELECT "ADMIN_ID" FROM "ADMINISTRATOR" WHERE "PASSWORD" = crypt(')" +
            password + "', \"PASSWORD\")");
    } catch (...) {
        server_request_log << "Failed attempt to log in as an administrator."
                           << std::endl;
        return false;
    }
    return true;
}

static bool check_dish_exists(const std::string &dish_id) {
    try {
        connectGet(R"(SELECT "PRICE" FROM "DISH" WHERE "DISH_ID" = ')" +
                   dish_id + "'");
    } catch (...) {
        return false;
    }
    return true;
}

static bool check_order_exists(const std::string &order_id) {
    try {
        connectGet(R"(SELECT "STATUS" FROM "ORDER" WHERE "ORDER_ID" = ')" +
                   order_id + "'");
    } catch (...) {
        return false;
    }
    return true;
}

static void getMenuHandler(const std::shared_ptr<restbed::Session> &session,
                           const std::shared_ptr<Server> &server);

static void getOrderHandler(const std::shared_ptr<restbed::Session> &session,
                            const std::shared_ptr<Server> &server);

static void getCartHandler(const std::shared_ptr<restbed::Session> &session,
                           const std::shared_ptr<Server> &server);

static void postAuthorizationMethodHandler(
    const std::shared_ptr<restbed::Session> &session,
    const std::string &data,
    const std::shared_ptr<Server> &server);

static void postCartMethodHandler(
    const std::shared_ptr<restbed::Session> &session,
    const std::string &data,
    const std::shared_ptr<Server> &server);

static void postOrderMethodHandler(
    const std::shared_ptr<restbed::Session> &session,
    const std::string &data,
    const std::shared_ptr<Server> &server);

static void notifySessionsMenuChanged() {
    folly::dynamic notificationJson = folly::dynamic::object;
    notificationJson["event"] = "menu_changed";
    notificationJson["timestamp"] =
        connectGet(R"(SELECT "TIMESTAMP" FROM "MENU_HISTORY")").c_str();

    restbes::server->pushToAllSessions(restbes::generateResponse(
        folly::toJson(notificationJson), "application/json",
        restbes::Connection::KEEP_ALIVE));
}

}  // namespace restbes
