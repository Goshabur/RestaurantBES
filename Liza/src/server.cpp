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
        item["dish_id"] = row[0].as<int>();
        item["name"] = row[1].as<std::string>();
        item["image"] = row[2].as<std::string>();
        item["price"] = row[3].as<int>();
        item["info"] = row[4].as<std::string>();
        item["status"] = row[5].as<int>();
        response["body"]["contents"].push_back(item);
    }

    return folly::toJson(response);
}

// std::string show_order_status(id_t order_id) {
//     std::string status =
//         connectGet(R"(SELECT "STATUS" FROM "ORDER" WHERE "ORDER_ID" = )" +
//                    std::to_string(order_id));
//
//     return orderStatuses[std::stoi(status)];
// }

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

void getMenuHandler(const std::shared_ptr<restbed::Session> &session,
                    const std::shared_ptr<Server> &server) {
    session->close(*generateResponse(folly::toJson(show_menu()),
                                     "application/json", Connection::CLOSE));
}

void getOrderHandler(const std::shared_ptr<restbed::Session> &session,
                     const std::shared_ptr<Server> &server) {
    auto request = session->get_request();
    std::string order_id = request->get_header("Order-ID", "");

    dynamic responseJson = dynamic::object;
    responseJson["query"] = "get_order";
    responseJson["status_code"] = 0;
    responseJson["body"] = dynamic::object;
    responseJson["body"]["item"] = "order";
    responseJson["body"]["order_id"] = order_id;
    responseJson["body"]["timestamp"] =
        restbes::Order::get_order_timestamp(order_id);
    responseJson["body"]["last_modified"] =
        restbes::Order::get_order_last_modified(order_id);
    responseJson["body"]["cost"] = restbes::Order::get_order_cost(order_id);
    responseJson["body"]["status"] = restbes::Order::get_order_status(order_id);
    responseJson["body"]["address"] =
        restbes::Order::get_order_address(order_id);
    responseJson["body"]["comment"] =
        restbes::Order::get_order_comment(order_id);
    responseJson["body"]["cart"] = dynamic::object;
    responseJson["body"]["cart"]["item"] = "cart";
    responseJson["body"]["cart"]["contents"] = dynamic::array;

    // TODO: вынести в функцию
    auto contents = json::parse(restbes::Order::get_order_items(order_id));
    for (auto &el : contents) {
        dynamic item = dynamic::object("dish_id", el.at("dish_id").get<int>())(
            "count", el.at("count").get<int>());
        responseJson["body"]["cart"]["contents"].push_back(item);
    }

    session->close(*generateResponse(folly::toJson(responseJson),
                                     "application/json", Connection::CLOSE));
}

void getCartHandler(const std::shared_ptr<restbed::Session> &session,
                    const std::shared_ptr<Server> &server) {
    auto request = session->get_request();
    std::string user_id = request->get_header("User-ID", "");

    dynamic responseJson = dynamic::object;
    responseJson["query"] = "get_cart";
    responseJson["status_code"] = 0;
    responseJson["body"] = dynamic::object;
    responseJson["body"]["item"] = "cart";
    responseJson["body"]["contents"] = dynamic::array;

    // TODO: аналогично
    auto contents = json::parse(restbes::Cart::get_cart(user_id));
    for (auto &el : contents) {
        dynamic item = dynamic::object("dish_id", el.at("dish_id").get<int>())(
            "count", el.at("count").get<int>());
        responseJson["body"]["contents"].push_back(item);
    }

    session->close(*generateResponse(folly::toJson(responseJson),
                                     "application/json", Connection::CLOSE));
}

void postAuthorizationMethodHandler(
    const std::shared_ptr<restbed::Session> &session,
    const std::string &data,
    const std::shared_ptr<Server> &server) {
    auto request = session->get_request();

    std::string user_id = request->get_header("User-ID", "");
    unsigned int session_id = request->get_header("Session-ID", 0);
    auto receivingSession = server->getSession(session_id);
    auto user = server->getUser(user_id);

    if (receivingSession == nullptr) {
        auto response = generateResponse("Check your connection", "text/plain",
                                         Connection::CLOSE);
        session->close(*response);
        return;
    }

    auto values = json::parse(data);
    std::string command = values.at("query").get<std::string>();
    std::string user_email = values.at("body").at("email").get<std::string>();
    std::string password = values.at("body").at("password").get<std::string>();

    dynamic responseJson = dynamic::object;
    responseJson["status_code"] = 0;
    responseJson["body"] = dynamic::object;
    responseJson["body"]["item"] = "user";

    if (command == "sign_in") {
        responseJson["query"] = "sign_in";
        std::string user_cart = "{}";

        if (check_sign_in(user_email, password)) {
            std::string user_name = restbes::Client::get_client_name(user_id);

            responseJson["body"]["user_id"] = std::stoi(user_id);
            responseJson["body"]["name"] = user_name;
            responseJson["body"]["email"] = user_email;
            responseJson["body"]["orders"] = dynamic::array;

            pqxx::result result = connectGet_pqxx_result(
                R"(SELECT * FROM "HISTORY" WHERE "CLIENT_ID" = )" + user_id);

            for (auto row : result) {
                dynamic item = dynamic::object;
                item["id"] = row[1].as<int>();
                item["status"] =
                    restbes::Order::get_order_status(item["id"].asString());
                item["timestamp"] =
                    restbes::Order::get_order_timestamp(item["id"].asString());
                item["last_modified"] = restbes::Order::get_order_last_modified(
                    item["id"].asString());
                responseJson["body"]["orders"].push_back(item);
            }

            session->close(*generateResponse(folly::toJson(responseJson) + '\n',
                                             "application/json",
                                             Connection::CLOSE));

            if (values.at("body").at("update_cart").get<int>() == 1) {
                auto new_cart = values.at("body").at("cart").get<std::string>();
                restbes::Cart::set_cart(user_id, new_cart,
                                        restbes::Cart::cart_cost(new_cart));

                dynamic notificationJson = dynamic::object;
                notificationJson["event"] = "cart_changed";
                notificationJson["timestamp"] = std::to_string(time_now);
                user->push(generateResponse(
                    folly::toJson(notificationJson) + '\n', "application/json",
                    Connection::KEEP_ALIVE));
            }

        } else {
            if (restbes::check_user_exists(user_email)) {
                responseJson["body"]["message"] = "error: incorrect password";
            } else {
                responseJson["body"]["message"] =
                    "error: no user with this email";
            }
            responseJson["status_code"] = 1;
            responseJson["body"] = dynamic::object;
            responseJson["body"]["error_code"] = 1;
            session->close(*generateResponse(folly::toJson(responseJson) + '\n',
                                             "application/json",
                                             Connection::CLOSE));
        }

    } else if (command == "sign_up") {
        responseJson["query"] = "sign_up";
        std::string user_name = values.at("name").get<std::string>();
        std::string user_cart = "{}";

        if (restbes::check_user_exists(user_email)) {
            responseJson["status_code"] = 1;
            responseJson["body"] = dynamic::object;
            responseJson["body"]["message"] =
                "error: user with this email already exists";
            responseJson["body"]["error_code"] = 1;

            session->close(*generateResponse(folly::toJson(responseJson) + '\n',
                                             "application/json",
                                             Connection::CLOSE));

        } else {
            if (values.at("body").at("update_cart").get<int>() == 1) {
                user_cart = values.at("body").at("cart").get<std::string>();
            }

            Client client(user_name, user_email, password, user_cart);
            user_id = client.get_client_id();

            responseJson["body"]["user_id"] = std::stoi(user_id);
            responseJson["body"]["name"] = user_name;
            responseJson["body"]["email"] = user_email;
            responseJson["body"]["orders"] = dynamic::array;

            session->close(*generateResponse(folly::toJson(responseJson) + '\n',
                                             "application/json",
                                             Connection::CLOSE));

            if (values.at("body").at("update_cart").get<int>() == 1) {
                auto new_cart = values.at("body").at("cart").get<std::string>();
                restbes::Cart::set_cart(client.get_client_id(), new_cart,
                                        restbes::Cart::cart_cost(new_cart));

                dynamic notificationJson = dynamic::object;
                notificationJson["event"] = "cart_changed";
                notificationJson["timestamp"] = std::to_string(time_now);

                user->push(generateResponse(folly::toJson(notificationJson),
                                            "application/json",
                                            Connection::KEEP_ALIVE));
            }
        }
    }
}

void postCartMethodHandler(const std::shared_ptr<restbed::Session> &session,
                           const std::string &data,
                           const std::shared_ptr<Server> &server) {
    auto request = session->get_request();
    std::string user_id = request->get_header("User-ID", "");
    unsigned int session_id = request->get_header("Session-ID", 0);
    auto receivingSession = server->getSession(session_id);
    auto user = server->getUser(user_id);

    if (receivingSession == nullptr) {
        auto response = generateResponse("Check your connection", "text/plain",
                                         Connection::CLOSE);
        session->close(*response);
        return;
    }

    auto values = json::parse(data);

    std::string command = values.at("query").get<std::string>();
    dynamic responseJson = dynamic::object;
    dynamic notificationJson = dynamic::object;
    responseJson["status_code"] = 0;
    responseJson["query"] = "cart_changed";
    notificationJson["event"] = "cart_changed";
    responseJson["timestamp"] = std::to_string(time_now);
    notificationJson["timestamp"] = std::to_string(time_now);

    if (command == "set_item_count") {
        int dish_id = values.at("body").at("dish_id").get<int>();
        int count = values.at("body").at("count").get<int>();

        restbes::Cart::set_item_count(user_id, dish_id, count);

        session->close(*generateResponse(folly::toJson(responseJson),
                                         "application/json",
                                         Connection::CLOSE));

        user->push(generateResponse(folly::toJson(notificationJson),
                                    "application/json",
                                    Connection::KEEP_ALIVE));

    } else if (command == "set_cart") {
        auto new_cart = values.at("body").at("cart").get<json>();

        restbes::Cart::set_cart(user_id, new_cart,
                                restbes::Cart::cart_cost(new_cart));

        session->close(*generateResponse(folly::toJson(responseJson),
                                         "application/json",
                                         Connection::CLOSE));

        user->push(generateResponse(folly::toJson(notificationJson),
                                    "application/json",
                                    Connection::KEEP_ALIVE));
    }
}

void postOrderMethodHandler(const std::shared_ptr<restbed::Session> &session,
                            const std::string &data,
                            const std::shared_ptr<Server> &server) {
    auto request = session->get_request();
    std::string user_id = request->get_header("User-ID", "");
    unsigned int session_id = request->get_header("Session-ID", 0);
    auto receivingSession = server->getSession(session_id);
    auto user = server->getUser(user_id);

    if (receivingSession == nullptr) {
        auto response = generateResponse("Check your connection", "text/plain",
                                         Connection::CLOSE);
        session->close(*response);
        return;
    }

    auto values = json::parse(data);
    //    std::string user_name = restbes::Client::get_client_name(user_id);
    //    std::string user_email = restbes::Client::get_client_name(user_id);
    //    std::string user_cart = restbes::Client::get_client_cart(user_id);
    std::string address = values.at("address").get<std::string>();
    std::string comment = values.at("comment").get<std::string>();

    dynamic responseJson = dynamic::object;
    responseJson["query"] = "create_order";
    responseJson["status_code"] = 0;
    responseJson["timestamp"] = std::to_string(restbes::time_now);

    restbes::Client client(std::stoi(user_id));
    std::string order_id = client.create_order(address, comment);

    dynamic notificationJson = dynamic::object;
    notificationJson["event"] = "order_changed";
    notificationJson["timestamp"] =
        restbes::Order::get_order_last_modified(order_id);
    notificationJson["body"] = dynamic::object;
    notificationJson["body"]["order_id"] = std::stoi(order_id);

    session->close(*generateResponse(folly::toJson(responseJson),
                                     "application/json", Connection::CLOSE));

    user->push(generateResponse(folly::toJson(notificationJson),
                                "application/json", Connection::KEEP_ALIVE));
}

}  // namespace restbes
