#pragma once

#include <fstream>
#include <string>
#include <vector>
#include "admin.h"
#include "pqxx/pqxx"

namespace restbes {

namespace {
enum OrderStatus { CREATED = 0, ACCEPTED, CANCELLED, IN_PROGRESS, COMPLETED };

[[maybe_unused]] std::vector<std::string> orderStatuses = {
    "CREATED", "ACCEPTED", "CANCELLED", "IN_PROGRESS", "COMPLETED"};

[[maybe_unused]] std::vector<std::string> dishStatuses = {"UNAVAILABLE",
                                                          "AVAILABLE"};

std::ofstream server_request_log("server_request_log.txt", std::ofstream::app);
std::ofstream server_error_log("server_error_log.txt", std::ofstream::app);
}  // namespace

static void connectExec(const std::string &sql) {
    pqxx::connection C(
        "dbname=testdb user=postgres password=restbes2022 hostaddr=127.0.0.1 "
        "port=5432");

    pqxx::work W(C);
    W.exec(sql);
    W.commit();

    // TODO: signal that something has changed and/or executed successfully

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

}  // namespace restbes
