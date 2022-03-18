#include "handlers.h"
#include "user.h"

using server_structure::Connection;

void getMethodHandler(std::shared_ptr<Session> session,
                      std::shared_ptr<Server> server) {
    std::string name = session->get_request()->get_header("Name");
    if (server->getUser(name) == nullptr) {
        server->addUser(name);
        auto user = server->getUser(name);
        user->addSession(session);
        user->yieldMessage("New user: " + name + '\n',
                           session->get_request()->get_path());
        user->yieldMessage("New receiving session for " + name + " at " +
                           session->get_request()->get_path() + '\n',
                           session->get_request()->get_path());
    }
    auto user = server->getUser(name);
    if (user->getSession(session) == nullptr) {
        user->addSession(session);
        user->yieldMessage("New receiving session for " + name + " at " +
                           session->get_request()->get_path() + '\n',
                           session->get_request()->get_path());
    }
}

void postEchoMethodHandler(std::shared_ptr<Session> session,
                           const std::string &data,
                           std::shared_ptr<Server> server) {
    auto request = session->get_request();
    std::string name = request->get_header("Name", "Anonymous");

    server->getUser(name)->yieldMessage("Your message: " + data + '\n',
                                        request->get_path());
    // make const, send JSONs
    std::string response_body = "PROCESSED, OK\n";
    auto response = generateResponse(response_body, Connection::CLOSE);
    session->close(*response);
}

void postMessengerMethodHandler(std::shared_ptr<Session> session,
                                const std::string &data,
                                std::shared_ptr<Server> server) {
    auto request = session->get_request();
    std::string name = request->get_header("Name", "Anonymous");

    for (const auto &user: server->getUsers()) {
        user.second->yieldMessage(name + ": " + data + '\n',
                                  request->get_path());
    }
    // make const, send JSONs
    std::string response_body = "PROCESSED, OK\n";
    auto response = generateResponse(response_body, Connection::CLOSE);
    session->close(*response);
}

void errorHandler(const int code,
                  const std::exception &exception,
                  std::shared_ptr<Session> session,
                  std::shared_ptr<Server> server) {}

void handleInactiveSessions(std::shared_ptr<Server> server) {
    for (const auto &user: server->getUsers()) {
        user.second->eraseInactiveSessions();
    }
}