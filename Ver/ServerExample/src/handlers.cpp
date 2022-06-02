#include "handlers.h"
#include "user.h"
#include "session.h"

#include "json.hpp"

#include <fstream>
#include <sstream>

using restbes::Connection;
using restbes::Session;
using restbes::Server;
using restbes::generateResponse;

void pollingHandler(std::shared_ptr<restbed::Session> session,
                    std::shared_ptr<Server> server) {
    std::string user_id = session->get_request()->get_header("User-ID",
                                                             "");
    unsigned int session_id = session->get_request()->get_header(
            "Session-ID",
            0);

    if (!user_id.empty() && server->getUser(user_id) == nullptr)
        // Unknown user
        server->addUser(user_id, server);
    auto realSession = server->getSession(session_id);
    if (realSession == nullptr) {
        // New session
        session_id = server->addSession(session, user_id);
        auto response = generateResponse(
                "New Session-ID: " + std::to_string(session_id) + '\n',
                "text/plain",
                Connection::KEEP_ALIVE);
        response->set_header("Session-ID", std::to_string(session_id));
        server->getSession(session_id)->push(response);
    } else {
        if (realSession->getSession() != session && realSession->is_closed()) {
            realSession->setSession(session);
            auto response = generateResponse(
                    "Reconnected session: " +
                    std::to_string(realSession->getId()) + '\n',
                    "text/plain", Connection::KEEP_ALIVE);
            if (session->is_open())
                session->yield(*response);
        }
        if (!user_id.empty() && realSession->getUserId().empty())
            server->assignSession(session_id, user_id);
    }
}

void postEchoMethodHandler(std::shared_ptr<restbed::Session> session,
                           const std::string &data,
                           std::shared_ptr<Server> server) {
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
    auto response = generateResponse("Your message: " + data + '\n',
                                     "text/plain", Connection::KEEP_ALIVE);
    if (user != nullptr) user->push(response);
    else receivingSession->push(response);

    // make const, send JSONs
    auto response2 = generateResponse("PROCESSED, OK\n", "text/plain",
                                      Connection::CLOSE);
    session->close(*response2);
}

void postMessengerMethodHandler(std::shared_ptr<restbed::Session> session,
                                const std::string &data,
                                std::shared_ptr<Server> server) {
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
    if (name.empty()) name = "Anon" + std::to_string(session_id);
    auto response = generateResponse(name + ": " + data + '\n', "text/plain",
                                     Connection::KEEP_ALIVE);
    server->pushToAllSessions(response);

    // make const, send JSONs
    auto response2 = generateResponse("PROCESSED, OK\n", "text/plain",
                                      Connection::CLOSE);
    session->close(*response2);
}

void errorHandler(const int code,
                  const std::exception &exception,
                  std::shared_ptr<restbed::Session> session,
                  std::shared_ptr<Server> server) {
    auto response = generateResponse("ERROR", "text/plain", Connection::CLOSE);
    session->close(*response);
}

void handleInactiveSessions(std::shared_ptr<Server> server) {
    auto lockedSessions = server->getSessionsW();
    for (auto session = lockedSessions->begin();
         session != lockedSessions->end();) {
        if (session->second->is_closed()) {
            session = lockedSessions->erase(session);
        } else ++session;
    }
}

void cleanUpUserSessions(std::shared_ptr<Server> server) {
    auto lockedUsers = server->getUsers();
    for (const auto &user: *lockedUsers) {
        user.second->eraseInactiveSessions();
    }
}

folly::Synchronized<std::string> &getMenu() {
    static std::fstream fs("/home/veronika/hse_study/RestaurantBES/Ver/ServerExample/exampleMenu.json");
    std::stringstream sstream;
    sstream << fs.rdbuf();
    static folly::Synchronized<std::string> menu{sstream.str()};
    return menu;
}

void getMenuHandler(std::shared_ptr<restbed::Session> session,
                    std::shared_ptr<Server> server) {
    auto response = generateResponse(*getMenu().rlock(), "application/json",
                                     Connection::CLOSE);
    session->close(*response);
}

void swapMenus(std::shared_ptr<Server> server) {
    static std::string menuFiles[2] = {
            "/home/veronika/hse_study/RestaurantBES/Ver/ServerExample/exampleMenu.json",
            "/home/veronika/hse_study/RestaurantBES/Ver/ServerExample/exampleMenu2.json"
    };
    static int i = 0;
    std::fstream fs(menuFiles[i++ & 1]);
    std::stringstream sstream;
    sstream << fs.rdbuf();
    *getMenu().wlock() = sstream.str();
    auto response = generateResponse(R"({"event":"menu_changed"})",
                                     "application/json",
                                     Connection::KEEP_ALIVE);
    server->pushToAllSessions(response);
}

void postUserHandler(std::shared_ptr<restbed::Session> session,
                     const std::string &data,
                     std::shared_ptr<Server> server) {
    static int counter = 0;
    nlohmann::json base = R"({
  "query": "",
  "status_code": 0,
  "body": {
    "item": "user",
    "user_id": 0,
    "name": "",
    "email": "",
    "orders": []
  }
})"_json;
    nlohmann::json js = nlohmann::json::parse(data);
    base["query"] = js["query"];
    base["body"]["user_id"] = ++counter;
    base["body"]["name"] = js["body"]["name"];
    base["body"]["email"] = js["body"]["email"];
    base["body"]["orders"] = R"(
[
    {
        "order_id": 123,
        "status": 1,
        "timestamp": 356845
    },
    {
        "order_id": 45,
        "status": 2,
        "timestamp": 980
    },
    {
        "order_id":7,
        "status": 1,
        "timestamp": 98770676
    }
]
)"_json;
    auto response = generateResponse(base.dump(), "application/json",
                                     Connection::CLOSE);
    session->close(*response);
}
