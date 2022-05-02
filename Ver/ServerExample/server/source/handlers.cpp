#include "handlers.h"
#include "user.h"
#include "session.h"

using restbes::Connection;
using restbes::Session;
using restbes::Server;
using restbes::generateResponse;

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
    if (user != nullptr) user->pushMessage(response);
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
    auto lockedSessions = server->getSessions();
    for (const auto &other_session: *(lockedSessions)) {
        other_session.second->push(response);
    }

    // make const, send JSONs
    auto response2 = generateResponse("PROCESSED, OK\n", "text/plain",
                                      Connection::CLOSE);
    session->close(*response2);
}

void errorHandler(const int code,
                  const std::exception &exception,
                  std::shared_ptr<restbed::Session> session,
                  std::shared_ptr<Server> server) {}

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