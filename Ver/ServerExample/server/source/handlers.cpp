#include "handlers.h"
#include "user.h"
#include "session.h"

using restbes::server_structure::Connection;
using restbes::session_structure::Session;
using restbes::server_structure::generateResponse;

void postEchoMethodHandler(std::shared_ptr<restbed::Session> session,
                           const std::string &data,
                           std::shared_ptr<Server> server) {
    auto request = session->get_request();
    std::string name = request->get_header("Name", "Anonymous");

    auto response = generateResponse("Your message: " + data + '\n', "text/plain", Connection::KEEP_ALIVE);
    server->getUser(name)->pushMessage(response);

    // make const, send JSONs
    auto response2 = generateResponse("PROCESSED, OK\n", "text/plain",
                                     Connection::CLOSE);
    session->close(*response2);
}

void postMessengerMethodHandler(std::shared_ptr<restbed::Session> session,
                                const std::string &data,
                                std::shared_ptr<Server> server) {
    auto request = session->get_request();
    std::string name = request->get_header("Name", "Anonymous");

    auto response = restbes::server_structure::generateResponse(name + ": " + data + '\n', "text/plain", Connection::KEEP_ALIVE);
    for (const auto &user: server->getUsers()) {
        user.second->pushMessage(response);
    }
    // make const, send JSONs
    auto response2 = generateResponse("PROCESSED, OK\n", "text/plain",
                                     Connection::CLOSE);
    session->close(*response);
}

void errorHandler(const int code,
                  const std::exception &exception,
                  std::shared_ptr<restbed::Session> session,
                  std::shared_ptr<Server> server) {}

void handleInactiveSessions(std::shared_ptr<Server> server) {
    for (const auto &user: server->getUsers()) {
        user.second->eraseInactiveSessions();
    }
}