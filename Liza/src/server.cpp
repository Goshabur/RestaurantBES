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
    pqxx::result result = connectGet_pqxx_result(sqlRequest);

    dynamic response = dynamic::object;
    response["query"] = "menu";
    response["status_code"] = 0;
    response["body"] = dynamic::object;
    response["body"]["item"] = "menu";
    response["body"]["timestamp"] =
        connectGet(R"(SELECT "TIMESTAMP" FROM "MENU_HISTORY")");
    response["body"]["contents"] = dynamic::array;

    for (auto row : result) {
        dynamic item = dynamic::object;
        item["item"] = "dish";
        item["id"] = row[0].as<int>();
        item["name"] = row[1].as<std::string>();
        item["image"] = row[2].as<std::string>();
        item["price"] = row[3].as<int>();
        item["info"] = row[4].as<std::string>();
        item["status"] = row[5].as<int>();
        response["body"]["contents"].push_back(item);
    }

    return folly::toJson(response);
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
    dynamic responseJson = dynamic::object;
    responseJson["status_code"] = 0;
    responseJson["body"] = dynamic::object;
    responseJson["body"]["item"] = "user";

    if (command == "sign_in") {
        responseJson["query"] = "sign_in";

        if (check_sign_in(user_email, password)) {
            user_name = restbes::connectGet(
                R"(SELECT "NAME" FROM "CLIENT" WHERE "EMAIL" = ')" +
                user_email + R"(' AND "PASSWORD" = crypt(')" + password + R"(',
                    "PASSWORD"))");

            user_id = restbes::connectGet(
                R"(SELECT "CLIENT_ID" FROM "CLIENT" WHERE "EMAIL" = ')" +
                user_email + R"(' AND "PASSWORD" = crypt(')" + password + R"(',
                    "PASSWORD"))");

            responseJson["body"]["user_id"] = std::stoi(user_id);
            responseJson["body"]["name"] = user_name;
            responseJson["body"]["email"] = user_email;
            responseJson["body"]["orders"] = dynamic::array;

            std::string sqlRequest =
                R"(SELECT * FROM "HISTORY" WHERE "CLIENT_ID" = )" + user_id;
            pqxx::result result = connectGet_pqxx_result(sqlRequest);

            for (auto row : result) {
                dynamic item = dynamic::object;
                item["id"] = row[1].as<int>();
                item["status"] = std::stoi(connectGet(
                    R"(SELECT "STATUS" FROM "ORDER" WHERE "ORDER_ID" = )" +
                    item["id"].asString()));
                item["timestamp"] = connectGet(
                    R"(SELECT "TIMESTAMP" FROM "ORDER" WHERE "ORDER_ID" = )" +
                    item["id"].asString());
                responseJson["body"]["orders"].push_back(item);
            }

            response = generateResponse(folly::toJson(responseJson) + '\n',
                                        "text/plain", Connection::KEEP_ALIVE);
        } else {
            // TODO: form an error
            response = generateResponse(folly::toJson(responseJson) + '\n',
                                        "text/plain", Connection::KEEP_ALIVE);
        }

    } else if (command == "sign_up") {
        responseJson["query"] = "sign_up";

        if (restbes::check_user_exists(user_email)) {
            // TODO: form an error
            response = generateResponse(folly::toJson(responseJson) + '\n',
                                        "text/plain", Connection::KEEP_ALIVE);
        } else {
            Client client(user_name, user_email, password, user_cart);

            responseJson["body"]["user_id"] = std::stoi(client.get_client_id());
            responseJson["body"]["name"] = user_name;
            responseJson["body"]["email"] = user_email;
            responseJson["body"]["orders"] = dynamic::array;

            std::string sqlRequest =
                R"(SELECT * FROM "HISTORY" WHERE "CLIENT_ID" = )" +
                client.get_client_id();
            pqxx::result result = connectGet_pqxx_result(sqlRequest);

            for (auto row : result) {
                dynamic item = dynamic::object;
                item["id"] = row[1].as<int>();
                item["status"] = std::stoi(connectGet(
                    R"(SELECT "STATUS" FROM "ORDER" WHERE "ORDER_ID" = )" +
                    item["id"].asString()));
                item["timestamp"] = connectGet(
                    R"(SELECT "TIMESTAMP" FROM "ORDER" WHERE "ORDER_ID" = )" +
                    item["id"].asString());
                responseJson["body"]["orders"].push_back(item);
            }

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
    dynamic responseJson = dynamic::object;
    responseJson["status_code"] = 0;
    responseJson["body"] = dynamic::object;
    responseJson["body"]["item"] = "cart";

    if (command == "add_to_cart") {
        client.add_to_cart(product_id);

    } else if (command == "delete_from_cart") {
        client.delete_from_cart(product_id);

    } else if (command == "clear_cart") {
        client.empty_cart();

    } else if (command == "show_cart") {
        responseJson["body"]["query"] = "show_cart";
        responseJson["body"]["contents"] = dynamic::array;

        std::string sqlRequest =
            R"(SELECT "CART" FROM "CART" WHERE "CLIENT_ID" = )" +
            client.get_client_id();
        std::string result = connectGet(sqlRequest);

        json cart(result);
        for (auto &el : cart.items()) {
            dynamic item = dynamic::object;
            item["id"] = el.key();
            item["quantity"] = el.value();
            responseJson["body"]["orders"].push_back(item);
        }

        response = generateResponse(folly::toJson(responseJson) + '\n',
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
