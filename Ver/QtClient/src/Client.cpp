
#include "Client.h"
#include "jsonParser.h"
#include "MenuList.h"

namespace restbes {

Client::Client(std::string server, int _port, QObject *parent)
        : address(
        "https://" + std::move(server) + ':' + std::to_string(_port)),
          port(_port),
          QObject(parent),
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
    sscanf(res->body.c_str(), "New Session-ID: %d", &newSessionId);
    setSessionId(newSessionId);
    headers.wlock()->find("Session-ID")->second = std::to_string(sessionId);
    startPolling();

    postingClient->set_read_timeout(180);
    postingClient->enable_server_certificate_verification(false);
    getMenuFromServer();
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
    auto response = postingClient->Post("/restbes",
                                        *headers.rlock(),
                                        JsonParser::generateRegistrationQuery(
                                                regEmail,
                                                regPassword,
                                                regName),
                                        "application/json");
    if (!response) return false;
    return parseUserFromJson(response->body);
}

bool Client::signInUser(const QString &regEmail, const QString &regPassword) {
    auto response = postingClient->Post("/restbes",
                                        *headers.rlock(),
                                        JsonParser::generateSignInQuery(
                                                regEmail,
                                                regPassword),
                                        "application/json");
    if (!response) return false;
    return parseUserFromJson(response->body);
}

bool Client::parseUserFromJson(const nlohmann::json &json) {
    if (json.at("status_code") != 0) return false;
    setRegStatus(true);
    const nlohmann::json::object_t &user = json.at("body");
    setUserId(user.at("user_id"));
    setName(JsonParser::getQStringValue(user, "name"));
    setEmail(JsonParser::getQStringValue(user, "email"));
    return true;
}

bool Client::parseUserFromJson(const std::string &input) {
    nlohmann::json json = nlohmann::json::parse(input);
    return parseUserFromJson(json);
}

MenuList *Client::getMenu() {
    return menuList.get();
}

void Client::startPolling() {
    pollingThread = std::make_shared<std::thread>([this]() {
        while (true) {
            auto res = pollingClient->Get("/get", *headers.rlock());
            if (res == nullptr) {
                throw std::runtime_error("Can't connect to the server");
            } else if (res->status != 200) {
                throw std::runtime_error(
                        "Bad response from the server " + std::to_string(res->status));
            }
            nlohmann::json json = nlohmann::json::parse(res->body);
            const std::string &stringEvent = json.at(
                    "event").get<std::string>();
            PollingEvent event = eventMap.at(stringEvent);
            switch (event) {
                case ItemCountChanged: {
                    break;
                }
                case OrderStatusChanged: {
                    break;
                }
                case NewOrder: {
                    break;
                }
                case AddMenuItem: {
                    break;
                }
                case RemoveMenuItem: {
                    break;
                }
                case ChangeMenuItem: {
                    break;
                }
                case ReloadMenu: {
                    getMenuFromServer();
                    break;
                }
                case ExampleEvent: {
                    emit exampleEvent();
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
    }
    else if (response->status != 200) {
        throw std::runtime_error("Can't get menu from the server");
    }

    auto menuData = JsonParser::parseMenu(response->body);
    if (!menuList) menuList = std::make_shared<MenuList>();
    menuList->setMenu(std::move(menuData));
}
}