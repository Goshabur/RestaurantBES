#include "handlers.h"
#include "user.h"
#include "session.h"

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