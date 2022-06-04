#include <QDebug>

#include "Client.h"
#include "jsonParser.h"
#include "MenuList.h"
#include "folly/dynamic.h"
#include "folly/json.h"

namespace restbes {

Client::Client(std::string server, int _port, QObject *parent)
        : QObject(parent),
          address("https://" + std::move(server) + ':' + std::to_string(_port)),
          port(_port),
          postingClient(std::make_shared<httplib::Client>(address)),
          pollingClient(std::make_shared<httplib::Client>(address)) {

    *headers.wlock() = {
            {"Session-ID", ""},
            {"User-ID",    ""}
    };
    pollingClient->enable_server_certificate_verification(false);
    pollingClient->set_keep_alive(true);
    pollingClient->set_read_timeout(180);
    auto res = pollingClient->Get("/get");
    if (res == nullptr) {
        throw std::runtime_error("Can't connect to the server");
    } else if (res->status != 200) {
        throw std::runtime_error(
                "Bad response from the server " + std::to_string(res->status));
    }

    int newSessionId;
    // TODO: answer from the server should be a JSON file
    sscanf(res->body.c_str(), "New Session-ID: %d", &newSessionId);
    setSessionId(newSessionId);
    qDebug() << "Got Session-ID from the server";
    qDebug() << res->body.c_str() << '\n';
    headers.wlock()->find("Session-ID")->second = std::to_string(sessionId);
    startPolling();

    postingClient->set_read_timeout(180);
    postingClient->enable_server_certificate_verification(false);
    getMenuFromServer();

    connect(this, &Client::getOrder, this, &Client::getOrderFromServer);
//    setItemCount(1, 2);
//    setItemCount(2, 1);
}

bool Client::getRegStatus() const {
    return regStatus;
}

void Client::setRegStatus(bool newStatus) {
    if (newStatus == regStatus) return;
    regStatus = newStatus;
    emit regStatusChanged();

}

QString Client::getName() const {
    return name;
}

void Client::setName(QString newName) {
    if (newName == name) return;
    name = std::move(newName);
    emit nameChanged();

}

QString Client::getEmail() const {
    return email;
}

void Client::setEmail(QString newEmail) {
    if (newEmail == email) return;
    email = std::move(newEmail);
    emit emailChanged();
}

int Client::getUserId() const {
    return userId;
}

void Client::setUserId(int newId) {
    if (newId == userId) return;
    userId = newId;
    emit userIdChanged();
}

int Client::getSessionId() const {
    return sessionId;
}

void Client::setSessionId(int newId) {
    if (newId == sessionId) return;
    sessionId = newId;
    emit sessionIdChanged();
}

bool Client::registerUser(const QString &regEmail,
                          const QString &regPassword,
                          const QString &regName) {
    // TODO: remove the next line
//    return false;

    std::string jsonReq = JsonParser::generateRegistrationQuery(
            regEmail,
            regPassword,
            regName,
            *cartList);
    qDebug() << jsonReq.c_str();
    auto response = postingClient->Post("/user",
                                        *headers.rlock(),
                                        jsonReq,
                                        "application/json");
    if (!response || response->status != 200) return false;
    qDebug() << "Authorized user";
    qDebug() << response->body.c_str() << '\n';

    // TODO: remove the next 3 lines
//    auto* order = new Order();
//    order->setCart(cartList.get());
//    emit orderStatusChanged(order);

    return parseUserFromJson(response->body);
}

bool Client::signInUser(const QString &regEmail, const QString &regPassword) {
    // TODO: remove the next line
//    return false;

    auto response = postingClient->Post("/user",
                                        *headers.rlock(),
                                        JsonParser::generateSignInQuery(
                                                regEmail,
                                                regPassword,
                                                *cartList),
                                        "application/json");
    if (!response || response->status != 200) return false;
    qDebug() << "Authorized user";
    qDebug() << response->body.c_str() << '\n';
    return parseUserFromJson(response->body);
}

bool Client::parseUserFromJson(const nlohmann::json &json) {
    if (json.at("status_code") != 0) return false;
    setRegStatus(true);
    const nlohmann::json &user = json.at("body");
    setUserId(user.at("user_id"));
    headers.wlock()->find("User-ID")->second = std::to_string(userId);
    setName(JsonParser::getQStringValue(user, "name"));
    setEmail(JsonParser::getQStringValue(user, "email"));
    orderList->setOrderData(JsonParser::parseOrderData(user["orders"]));
    return true;
}

bool Client::parseUserFromJson(const std::string &input) {
    nlohmann::json json = nlohmann::json::parse(input);
    return parseUserFromJson(json);
}

MenuList *Client::getMenu() const {
    return menuList.get();
}

void Client::startPolling() {
    pollingThread = std::make_shared<std::thread>([this]() {
        while (true) {
            auto res = pollingClient->Get("/get", headers.copy());
            if (res == nullptr) {
                throw std::runtime_error("Can't connect to the server");
            } else if (res->status != 200) {
                throw std::runtime_error(
                        "Bad response from the server " +
                        std::to_string(res->status));
            }
            qDebug() << res->body.c_str();
            nlohmann::json json = nlohmann::json::parse(res->body);
            const std::string &stringEvent = json.at(
                    "event").get<std::string>();
            unsigned int timestamp = json["timestamp"].get<unsigned int>();
            PollingEvent event = eventMap.at(stringEvent);
            switch (event) {
                case CartChanged: {
                    if (timestamp <= cartList->getTimestamp()) break;
                    if (regStatus) getCartFromServer();
                    break;
                }
                case OrderChanged: {
//                    if (timestamp <= orderList->getTimestamp()) {
//                      qDebug() << "timestamp is:" << timestamp;
//                      qDebug() << "old timestamp is:" << orderList->getTimestamp();
//                      break;
//                    }
                    int orderId = json["body"]["order_id"].get<int>();
                    emit getOrder(orderId);
                    break;
                }
                case MenuChanged: {
                    if (timestamp <= menuList->getTimestamp()) break;
                    getMenuFromServer();
                    break;
                }
                default:
                    break;
            }
        }
    });
}

restbes::CartList *Client::getCart() const {
    return cartList.get();
}

void Client::getMenuFromServer() {
    auto response = postingClient->Get("/menu",
                                       *headers.rlock());
    if (!response) {
        throw std::runtime_error("Can't connect to resource /menu");
    } else if (response->status != 200) {
        throw std::runtime_error("Can't get menu from the server");
    }
    qDebug() << "Got menu from the server";
    qDebug() << response->body.c_str() << '\n';

    nlohmann::json jsonMenu = nlohmann::json::parse(response->body);
    auto menuData = JsonParser::parseMenu(jsonMenu["body"]);
    unsigned int timestamp = jsonMenu["body"]["timestamp"].get<unsigned int>();
    menuList->setMenu(std::move(menuData));
    menuList->setTimestamp(timestamp);
}

void Client::clearCart(bool notifyServer) {
    cartList->clearCart();
    if (regStatus && notifyServer) {
        std::string query = JsonParser::generateSetCartQuery(*cartList);
        auto response = postingClient->Post("/cart",
                                            *headers.rlock(),
                                            query,
                                            "application/json");
        qDebug() << "set_cart command sent to the server";
        qDebug() << query.c_str() << '\n';

        if (!response) {
            qDebug() << "Can't connect to the server\n";
            return;
        }
        if (response->status != 200) {
            qDebug() << "Bad response\n" << response->body.c_str() << '\n';
            return;
        }
        nlohmann::json json = nlohmann::json::parse(response->body);
        unsigned int timestamp = json["timestamp"].get<int>();
        cartList->setTimestamp(timestamp);
        qDebug() << "Answer:\n" << response->body.c_str() << '\n';
    }
}

void Client::getCartFromServer() {
    auto response = postingClient->Get("/cart",
                                       *headers.rlock());
    if (!response) {
        throw std::runtime_error("Can't connect to the server");
    } else if (response->status != 200) {
        throw std::runtime_error("Can't get cart from /cart");
    }
    qDebug() << "Got cart from the server";
    qDebug() << response->body.c_str() << '\n';

    nlohmann::json jsonBody = nlohmann::json::parse(response->body);
    auto cartData = JsonParser::parseCart(jsonBody.at("body"));
//    unsigned int timestamp = jsonBody["body"]["timestamp"].get<int>();
    cartList->setCart(std::move(cartData));
//    cartList->setTimestamp(timestamp);
}

void Client::setItemCount(int id, int value) {
    bool countChanged = cartList->setItemCount(id, value);
    if (regStatus && countChanged) {
        std::string query = JsonParser::generateSetItemCountQuery(id, value);
        auto response = postingClient->Post("/cart",
                                            *headers.rlock(),
                                            query,
                                            "application/json");
        qDebug() << "set_item_count command sent to the server";
        qDebug() << query.c_str() << '\n';

        if (!response) {
            qDebug() << "Can't connect to the server\n";
            return;
        }
        if (response->status != 200) {
            qDebug() << "Bad response\n" << response->body.c_str() << '\n';
            return;
        }
        nlohmann::json json = nlohmann::json::parse(response->body);
        unsigned int timestamp = json["timestamp"].get<unsigned int>();
        cartList->setTimestamp(timestamp);
        qDebug() << "Answer:\n" << response->body.c_str() << '\n';
    }
}

void Client::increaseItemCount(int id) {
    setItemCount(id, cartList->getItemCount(id) + 1);
}

void Client::decreaseItemCount(int id) {
    setItemCount(id, cartList->getItemCount(id) - 1);
}

void Client::createOrder(const QString &addr, const QString &commnt) {
    std::string query = JsonParser::generateCreateOrderQuery(addr, commnt);
    auto response = postingClient->Post("/order",
                                        *headers.rlock(),
                                        query,
                                        "application/json");
    qDebug() << "New order sent to the server";
    qDebug() << query.c_str() << '\n';
}

Client::Client(QObject *parent) : QObject(parent) {
}

void Client::getOrderFromServer(int orderId) {
    auto orderHeaders = headers.copy();
    orderHeaders.insert({"Order-ID", std::to_string(orderId)});
    auto response = postingClient->Get("/order",
                                       orderHeaders);
    if (!response) {
        throw std::runtime_error("Can't connect to the server");
    } else if (response->status != 200 || response->body.empty()) {
        qDebug() << "Can't get the order" << orderId << "from /order\n";
        return;
    }
    qDebug() << "Got the order " << orderId << " from the server";
    qDebug() << response->body.c_str() << '\n';

    nlohmann::json jsonBody = nlohmann::json::parse(response->body);
    auto *order = new Order();
    JsonParser::parseOrder(jsonBody["body"], *order);
    orderList->setItemStatus(order->getOrderId(), order->getStatus(), order->getTimestamp());
    orderList->setTimestamp(order->getLastModified());
    if (order->getStatus() == 0) emit newOrder(order);
    else emit orderStatusChanged(order);
}

OrderList *Client::getOrderList() const {
    return orderList.get();
}

}