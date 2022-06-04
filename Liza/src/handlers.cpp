#include "handlers.h"
#include "../../Liza/include/fwd.h"
#include "client.h"
#include "order.h"
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

std::shared_ptr<Server> &getServer() {
    static auto server = std::make_shared<Server>();
    return server;
}

void addUserToServer(const std::shared_ptr<Server> &server,
                     const std::shared_ptr<Session> &receivingSession,
                     const std::string &user_id,
                     unsigned int &session_id) {
    Server::addUser(user_id, server);
    if (receivingSession->getUserId().empty())
        server->assignSession(session_id, user_id);
}

void sendResponse(const std::shared_ptr<restbed::Session> &session,
                  const dynamic &responseJson) {
    session->close(*generateResponse(folly::toJson(responseJson),
                                     "application/json", Connection::CLOSE));
}

void sendNotification(const std::shared_ptr<User> &user,
                      const dynamic &notificationJson) {
    user->push(generateResponse(folly::toJson(notificationJson),
                                "application/json", Connection::KEEP_ALIVE));
}

void parseInsertOrders(dynamic &responseJson, const std::string &user_id) {
    pqxx::result result = connectGet_pqxx_result(
        R"(SELECT * FROM "HISTORY" WHERE "CLIENT_ID" = )" + user_id);

    for (auto row : result) {
        pqxx::result result_order = connectGet_pqxx_result(
            R"(SELECT * FROM "ORDER" WHERE "ORDER_ID" = )" +
            std::to_string(row[1].as<int>()));
        dynamic item = dynamic::object;
        item["order_id"] = row[1].as<int>();
        item["status"] = restbesOrder::get_order_status(
            std::to_string(item["order_id"].asInt()));
        item["timestamp"] = restbesOrder::get_order_timestamp(
            std::to_string(item["order_id"].asInt()));
        item["last_modified"] = restbesOrder::get_order_last_modified(
            std::to_string(item["order_id"].asInt()));
        responseJson["body"]["orders"].push_back(item);
    }
}

dynamic cartChangeResponse() {
    return dynamic::object("status_code", 0)("query", "cart_changed")(
        "timestamp", restbes::getTime());
}

dynamic cartChangeNotification() {
    return dynamic::object("event", "cart_changed")("timestamp",
                                                    restbes::getTime());
}

dynamic orderChangedResponse() {
    return dynamic::object("query", "create_order")("status_code", 0)(
        "timestamp", restbes::getTime());
}

dynamic orderChangedNotification(const std::string &order_id) {
    return dynamic::object("event", "order_changed")(
        "timestamp", restbesOrder::get_order_last_modified(order_id))(
        "body", dynamic::object("order_id", std::stoi(order_id)));
}

void formErrorResponse(dynamic &responseJson, const std::string &user_email) {
    if (restbesClient::check_user_exists(user_email)) {
        responseJson["body"]["message"] = "error: incorrect password";
    } else {
        responseJson["body"]["message"] = "error: no user with this email";
    }
    responseJson["status_code"] = 1;
    responseJson["body"] = dynamic::object;
    responseJson["body"]["error_code"] = 1;
}

void setUsersInfoInResponse(dynamic &responseJson,
                            const std::string &user_id,
                            const std::string &user_name,
                            const std::string &user_email) {
    responseJson["body"]["user_id"] = std::stoi(user_id);
    responseJson["body"]["name"] = user_name;
    responseJson["body"]["email"] = user_email;
    responseJson["body"]["orders"] = dynamic::array;
}

void postAuthorizationMethodHandler(
    const std::shared_ptr<restbed::Session> &session,
    const std::string &data,
    const std::shared_ptr<Server> &server) {
    auto request = session->get_request();

    std::string user_id = request->get_header("User-ID", "");
    unsigned int session_id = request->get_header("Session-ID", 0);
    auto receivingSession = server->getSession(session_id);

    if (receivingSession == nullptr) {
        session->close(*generateResponse("Check your connection", "text/plain",
                                         Connection::CLOSE));
        return;
    }

    auto values = json::parse(data);
    std::string command = values.at("query").get<std::string>();
    std::string user_email = values.at("body").at("email").get<std::string>();
    std::string password = values.at("body").at("password").get<std::string>();

    dynamic responseJson = dynamic::object("status_code", 0)(
        "body", dynamic::object("item", "user"));

    if (command == "sign_in") {
        responseJson["query"] = "sign_in";

        if (restbesClient::check_sign_in(user_email, password)) {
            user_id = restbesClient::get_client_id_by_email(user_email);
            std::string user_name = restbesClient::get_client_name(user_id);

            addUserToServer(server, receivingSession, user_id, session_id);
            auto user = server->getUser(user_id);

            setUsersInfoInResponse(responseJson, user_id, user_name, user_email);

            parseInsertOrders(responseJson, user_id);

            sendResponse(session, responseJson);

            if (values.at("body").at("update_cart").get<bool>()) {
                auto new_cart = values.at("body").at("cart").get<json>().dump();
                restbesCart::set_cart(user_id, new_cart,
                                      restbesCart::cart_cost(new_cart));

                dynamic notificationJson = cartChangeNotification();
                sendNotification(user, notificationJson);
            }

        } else {
            formErrorResponse(responseJson, user_email);
            sendResponse(session, responseJson);
        }

    } else if (command == "sign_up") {
        std::string user_name = values.at("body").at("name").get<std::string>();
        responseJson["query"] = "sign_up";
        std::string user_cart = "{}";

        if (restbesClient::check_user_exists(user_email)) {
            responseJson["status_code"] = 1;
            responseJson["body"] = dynamic::object;
            responseJson["body"]["message"] =
                "error: user with this email already exists";
            responseJson["body"]["error_code"] = 1;

            sendResponse(session, responseJson);

        } else {
            if (values.at("body").at("update_cart").get<bool>()) {
                user_cart = values.at("body").at("cart").get<json>().dump();
            }

            restbesClient::Client client(user_name, user_email, password,
                                         user_cart);
            user_id = client.get_client_id();

            addUserToServer(server, receivingSession, user_id, session_id);
            auto user = server->getUser(user_id);

            setUsersInfoInResponse(responseJson, user_id, user_name, user_email);

            sendResponse(session, responseJson);

            if (values.at("body").at("update_cart").get<bool>()) {
                dynamic notificationJson = dynamic::object;
                notificationJson["event"] = "cart_changed";
                notificationJson["timestamp"] = restbes::getTime();

                sendNotification(user, notificationJson);
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
        session->close(*generateResponse("Check your connection", "text/plain",
                                         Connection::CLOSE));
        return;
    }

    auto values = json::parse(data);
    std::string command = values.at("query").get<std::string>();

    dynamic responseJson = cartChangeResponse();
    dynamic notificationJson = cartChangeNotification();

    if (command == "set_item_count") {
        restbesCart::set_item_count(user_id,
                                    values.at("body").at("dish_id").get<int>(),
                                    values.at("body").at("count").get<int>());

        sendResponse(session, responseJson);
        sendNotification(user, notificationJson);

    } else if (command == "set_cart") {
        std::string new_cart = values.at("body").at("cart").get<json>().dump();

        restbesCart::set_cart(user_id, new_cart,
                              restbesCart::cart_cost(new_cart));

        sendResponse(session, responseJson);
        sendNotification(user, notificationJson);
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
        session->close(*generateResponse("Check your connection", "text/plain",
                                         Connection::CLOSE));
        return;
    }

    auto values = json::parse(data);
    std::string address = values.at("body").at("address").get<std::string>();
    std::string comment = values.at("body").at("comment").get<std::string>();

    restbesClient::Client client(std::stoi(user_id));
    std::string order_id = client.create_order(address, comment);

    dynamic responseJson = orderChangedResponse();
    dynamic notificationJson = orderChangedNotification(order_id);

    sendResponse(session, responseJson);
    sendNotification(user, notificationJson);
}

void getMenuHandler(const std::shared_ptr<restbed::Session> &session,
                    const std::shared_ptr<Server> &server) {
    session->close(
        *generateResponse(show_menu(), "application/json", Connection::CLOSE));
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
    responseJson["body"]["order_id"] = std::stoi(order_id);
    responseJson["body"]["timestamp"] =
        restbesOrder::get_order_timestamp(order_id);
    responseJson["body"]["last_modified"] =
        restbesOrder::get_order_last_modified(order_id);
    responseJson["body"]["cost"] = restbesOrder::get_order_cost(order_id);
    responseJson["body"]["status"] = restbesOrder::get_order_status(order_id);
    responseJson["body"]["address"] = restbesOrder::get_order_address(order_id);
    responseJson["body"]["comment"] = restbesOrder::get_order_comment(order_id);
    responseJson["body"]["cart"] = dynamic::object;
    responseJson["body"]["cart"]["item"] = "cart";
    responseJson["body"]["cart"]["contents"] = dynamic::array;

    auto contents = json::parse(restbesOrder::get_order_items(order_id));
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

    auto contents = json::parse(restbesCart::get_cart(user_id));
    for (auto &el : contents) {
        dynamic item = dynamic::object("dish_id", el.at("dish_id").get<int>())(
            "count", el.at("count").get<int>());
        responseJson["body"]["contents"].push_back(item);
    }

    session->close(*generateResponse(folly::toJson(responseJson),
                                     "application/json", Connection::CLOSE));
}

void errorHandler(const int code,
                  const std::exception &exception,
                  const std::shared_ptr<restbed::Session> &session,
                  const std::shared_ptr<Server> &server) {
    auto response = generateResponse("ERROR", "text/plain", Connection::CLOSE);
    session->close(*response);
}

void handleInactiveSessions(const std::shared_ptr<Server> &server) {
    auto lockedSessions = server->getSessionsW();
    for (auto session = lockedSessions->begin();
         session != lockedSessions->end();) {
        if (session->second->is_closed()) {
            session = lockedSessions->erase(session);
        } else
            ++session;
    }
}

void cleanUpUserSessions(const std::shared_ptr<Server> &server) {
    auto lockedUsers = server->getUsers();
    for (const auto &user : *lockedUsers) {
        user.second->eraseInactiveSessions();
    }
}

void pollingHandler(const std::shared_ptr<restbed::Session> &session,
                    const std::shared_ptr<Server> &server) {
    std::string user_id = session->get_request()->get_header("User-ID", "");
    unsigned int session_id =
        session->get_request()->get_header("Session-ID", 0);

    if (!user_id.empty() && server->getUser(user_id) == nullptr) {
        server->addUser(user_id, server);
    }
    auto realSession = server->getSession(session_id);
    if (realSession == nullptr) {
        session_id = server->addSession(session, user_id);
        auto response = generateResponse(
            "New Session-ID: " + std::to_string(session_id) + '\n',
            "text/plain", Connection::KEEP_ALIVE);
        response->set_header("Session-ID", std::to_string(session_id));
        server->getSession(session_id)->push(response);
    } else {
        if (realSession->getSession() != session && realSession->is_closed()) {
            realSession->setSession(session);
            auto response = generateResponse(
                "Reconnected session: " + std::to_string(realSession->getId()) +
                    '\n',
                "text/plain", Connection::KEEP_ALIVE);
            if (session->is_open())
                session->yield(*response);
        }
        if (!user_id.empty() && realSession->getUserId().empty())
            server->assignSession(session_id, user_id);
    }
}

void notifySessionsMenuChanged() {
    folly::dynamic notificationJson = folly::dynamic::object;
    notificationJson["event"] = "menu_changed";
    notificationJson["timestamp"] =
        std::stoi(connectGet(R"(SELECT "TIMESTAMP" FROM "MENU_HISTORY")"));

    restbes::getServer()->pushToAllSessions(restbes::generateResponse(
        folly::toJson(notificationJson), "application/json",
        restbes::Connection::KEEP_ALIVE));
}

void notifySessionsOrderChanged(const std::string &order_id) {
    folly::dynamic notificationJson = folly::dynamic::object;
    notificationJson["event"] = "order_changed";
    notificationJson["timestamp"] =
        restbesOrder::get_order_last_modified(order_id);
    notificationJson["body"] = folly::dynamic::object;
    notificationJson["body"]["order_id"] = std::stoi(order_id);

    std::string user_id = restbesOrder::get_order_client_id(order_id);
    Server::addUser(user_id, getServer());
    auto user = restbes::getServer()->getUser(user_id);

    user->push(restbes::generateResponse(folly::toJson(notificationJson),
                                         "application/json",
                                         restbes::Connection::KEEP_ALIVE));
}

std::string show_menu() {
    std::string sqlRequest = R"(SELECT * FROM "DISH" WHERE "STATUS" = 1)";
    pqxx::result result = connectGet_pqxx_result(sqlRequest);

    dynamic response = dynamic::object;
    response["query"] = "menu";
    response["status_code"] = 0;
    response["body"] = dynamic::object;
    response["body"]["item"] = "menu";
    response["body"]["timestamp"] =
        std::stoi(connectGet(R"(SELECT "TIMESTAMP" FROM "MENU_HISTORY")"));
    response["body"]["contents"] = dynamic::array;

    for (auto row : result) {
        dynamic item = dynamic::object;
        item["item"] = "dish";
        item["dish_id"] = row[0].as<int>();
        item["name"] = row[1].as<std::string>();
        item["image"] = row[2].as<std::string>();
        item["price"] = row[3].as<int>();
        item["status"] = row[4].as<int>();
        response["body"]["contents"].push_back(item);
    }
    return folly::toJson(response);
}

}  // namespace restbes
