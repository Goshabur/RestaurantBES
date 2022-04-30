#include "user.h"
#include "server.h"
#include "session.h"

namespace restbes::user_structure {

using server_structure::Connection;

User::User(std::string nm) : id(std::move(nm)) {}

void User::pushMessage(std::shared_ptr<restbed::Response> response) {
    auto lockedSessions = activeSessions.rlock();
    for (const auto &session: *lockedSessions) {
        session.second->push(response);
    }
}

void User::addSession(std::shared_ptr<restbed::Session> session) {
    activeSessions.wlock()->insert({session, std::make_shared<Session>(session, id.copy())});
}

void User::eraseInactiveSessions() {
    auto lockedSessions = activeSessions.wlock();
    for (auto session = lockedSessions->begin();
         session != lockedSessions->end();) {
        if (session->second->is_closed()) {
            session = lockedSessions->erase(session);
        } else {
            ++session;
        }
    }
}

std::shared_ptr<Session>
User::getSession(std::shared_ptr<restbed::Session> ss) const {
    auto lockedSessions = activeSessions.rlock();
    auto session = lockedSessions->find(ss);
    if (session != lockedSessions->end()) return session->second;
    return nullptr;
}

std::unordered_map<std::shared_ptr<restbed::Session>, std::shared_ptr<Session>>
User::getActiveSessions() const {
    return *(activeSessions.rlock());
}

const std::string &User::getId() const {
    return *(id.rlock());
}

} //user_structure
