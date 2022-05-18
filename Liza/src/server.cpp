#include "server.h"
#include <fstream>
#include <iostream>
#include <restbed>
#include "../include/admin.h"
#include "../include/client.h"
#include "../include/fwd.h"
#include "../include/order.h"
#include "folly/dynamic.h"
#include "handlers.h"
#include "session.h"
#include "user.h"

using folly::dynamic;
using folly::parseJson;
using nlohmann::json;
using restbes::Connection;
using restbes::generateResponse;
using restbes::Server;
using restbes::server_error_log;
using restbes::server_request_log;
using restbes::Session;

namespace restbes {

std::string show_menu() {
    std::string sqlRequest = R"(SELECT * FROM "DISH" WHERE "STATUS" = 1)";
    pqxx::connection C(
        "dbname=testdb user=postgres password=restbes2022 hostaddr=127.0.0.1 "
        "port=5432");

    pqxx::work W(C);
    pqxx::result result(W.exec(sqlRequest));
    C.disconnect();

    dynamic menu = dynamic::object;
    for (auto row : result) {
        if (row[5].as<int>() == 1) {
            menu[row[1].as<std::string>()] = row[3].as<int>();
        }
    }
    auto menuStr = folly::toJson(menu);

    return menuStr;
}

std::string show_order_status(id_t order_id) {
    std::string status =
        connectGet(R"(SELECT "STATUS" FROM "ORDER" WHERE "ORDER_ID" = )" +
                   std::to_string(order_id));

    return orderStatuses[std::stoi(status)];
}

bool check_user_exists(const std::string &email) {
    try {
        connectGet(R"(SELECT "CLIENT_ID" FROM "CLIENT" WHERE "EMAIL" = ')" +
                   email + "'");
    } catch (const pqxx::pqxx_exception &e) {
        server_error_log << e.base().what() << std::endl;
        const auto *s = dynamic_cast<const pqxx::sql_error *>(&e.base());
        if (s)
            server_request_log << "No such user, request: " << s->query()
                               << std::endl;
        return false;
    }
    return true;
}

bool check_sign_in(const std::string &email, const std::string &password) {
    try {
        connectGet(R"(SELECT "CLIENT_ID" FROM "CLIENT" WHERE "EMAIL" = ')" +
                   email + "' AND \"PASSWORD\" = crypt('" + password +
                   "', \"PASSWORD\")");
    } catch (const pqxx::pqxx_exception &e) {
        server_error_log << e.base().what() << std::endl;
        const auto *s = dynamic_cast<const pqxx::sql_error *>(&e.base());
        if (s)
            server_request_log
                << "Wrong password/email or no such user, request: "
                << s->query() << std::endl;
        return false;
    }
    return true;
}

void postAuthorizationMethodHandler(
    const std::shared_ptr<restbed::Session> &session,
    const std::string &data,
    const std::shared_ptr<Server> &server) {
    auto request = session->get_request();
    std::string name = request->get_header("User-ID", "");
    unsigned int session_id = request->get_header("Session-ID", 0);
    auto receivingSession = server->getSession(session_id);
    auto user = server->getUser(name);

    if (receivingSession == nullptr) {
        auto response = generateResponse("Invalid Session-ID", "text/plain",
                                         Connection::CLOSE);
        session->close(*response);
        return;
    }

    json values(data);
    dynamic command = values.at("command");
    std::string user_id = values.at("user_id");
    std::string user_name = values.at("name");
    std::string user_email = values.at("email");
    std::string password = values.at("password");
    std::string user_cart = values.at("cart");

    std::shared_ptr<restbed::Response> response;

    if (command == "sign_in") {
        if (check_sign_in(user_email, password)) {
            user_name = restbes::connectGet(
                R"(SELECT "NAME" FROM "CLIENT" WHERE "EMAIL" = ')" +
                user_email + R"(' AND "PASSWORD" = crypt(')" + password + R"(',
                    "PASSWORD"))");

            user_id = restbes::connectGet(
                R"(SELECT "CLIENT_ID" FROM "CLIENT" WHERE "EMAIL" = ')" +
                user_email + R"(' AND "PASSWORD" = crypt(')" + password + R"(',
                    "PASSWORD"))");

            dynamic responseJson =
                dynamic::object(true, dynamic::array(user_id, user_name));
            response = generateResponse(folly::toJson(responseJson) + '\n',
                                        "text/plain", Connection::KEEP_ALIVE);
        } else {
            dynamic responseJson = dynamic::object(false, dynamic::array(""));
            response = generateResponse(folly::toJson(responseJson) + '\n',
                                        "text/plain", Connection::KEEP_ALIVE);
        }

    } else if (command == "sign_up") {
        if (restbes::check_user_exists(user_email)) {
            dynamic responseJson = dynamic::object(false, dynamic::array(""));
            response = generateResponse(folly::toJson(responseJson) + '\n',
                                        "text/plain", Connection::KEEP_ALIVE);
        } else {
            Client client(user_name, user_email, password, user_cart);

            dynamic responseJson =
                dynamic::object(true, dynamic::array(client.get_client_id()));
            response = generateResponse(folly::toJson(responseJson) + '\n',
                                        "text/plain", Connection::KEEP_ALIVE);
        }
    }

    user->pushMessage(response);
    session->close(
        *generateResponse("PROCESSED, OK\n", "text/plain", Connection::CLOSE));
}

void postCartMethodHandler(const std::shared_ptr<restbed::Session> &session,
                           const std::string &data,
                           const std::shared_ptr<Server> &server) {
    auto request = session->get_request();
    std::string name = request->get_header("User-ID", "");
    unsigned int session_id = request->get_header("Session-ID", 0);
    auto receivingSession = server->getSession(session_id);
    auto user = server->getUser(name);

    if (receivingSession == nullptr) {
        auto response = generateResponse("Invalid Session-ID", "text/plain",
                                         Connection::CLOSE);
        session->close(*response);
        return;
    }

    json values(data);
    dynamic command = values.at("command");
    id_t user_id = static_cast<id_t>(values.at("user_id"));
    id_t product_id = static_cast<id_t>(values.at("product_id"));
    Client client(user_id);

    std::shared_ptr<restbed::Response> response;

    if (command == "add_to_cart") {
        client.add_to_cart(product_id);

    } else if (command == "delete_from_cart") {
        client.delete_from_cart(product_id);

    } else if (command == "clear_cart") {
        client.empty_cart();

    } else if (command == "show_cart") {
        response = generateResponse(restbes::Cart::get_cart(user_id) + '\n',
                                    "text/plain", Connection::KEEP_ALIVE);
    }

    user->pushMessage(response);
    session->close(
        *generateResponse("PROCESSED, OK\n", "text/plain", Connection::CLOSE));
}

void postOrderMethodHandler(const std::shared_ptr<restbed::Session> &session,
                            const std::string &data,
                            const std::shared_ptr<Server> &server) {
    auto request = session->get_request();
    std::string name = request->get_header("User-ID", "");
    unsigned int session_id = request->get_header("Session-ID", 0);
    auto receivingSession = server->getSession(session_id);
    auto user = server->getUser(name);

    if (receivingSession == nullptr) {
        auto response = generateResponse("Invalid Session-ID", "text/plain",
                                         Connection::CLOSE);
        session->close(*response);
        return;
    }

    json values(data);
    dynamic command = values.at("command");
    std::string user_name = values.at("name");
    std::string user_email = values.at("email");
    std::string password = values.at("password");
    // if anonymous, send me their cart
    std::string user_cart = values.at("cart");
    id_t user_id = static_cast<id_t>(values.at("user_id"));
    id_t order_id = static_cast<id_t>(values.at("order_id"));

    std::shared_ptr<restbed::Response> response;

    Client client;
    if (user != nullptr)
        client = Client(user_id);
    else
        client = Client(user_name, user_email, password, user_cart);

    if (command == "create_order") {
        client.create_order();

    } else if (command == "show_order_status") {
        response = generateResponse(
            restbes::Client::show_order_status(order_id) + '\n', "text/plain",
            Connection::KEEP_ALIVE);
    }

    user->pushMessage(response);
    session->close(
        *generateResponse("PROCESSED, OK\n", "text/plain", Connection::CLOSE));
}

}  // namespace restbes
