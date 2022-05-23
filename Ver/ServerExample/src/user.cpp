#include "user.h"
#include "server.h"
#include "session.h"

namespace restbes {

User::User(std::string nm, std::shared_ptr<Server> serv) : id(std::move(nm)),
                                                           server(std::move(
                                                                   serv)) {}

void User::push(std::shared_ptr<restbed::Response> response) {
    auto lockedSessions = activeSessions.rlock();
    for (auto session_id: *lockedSessions) {
        auto session = server->getSession(session_id);
        if (session != nullptr) session->push(response);
    }
}

void User::addSession(unsigned int session_id) {
    activeSessions.wlock()->insert(session_id);
    server->getSession(session_id)->setUser(id.copy());
}

void User::eraseInactiveSessions() {
    auto lockedSessions = activeSessions.wlock();
    for (auto session_id = lockedSessions->begin();
         session_id != lockedSessions->end();) {
        if (server->getSession(*session_id) == nullptr) {
            session_id = lockedSessions->erase(session_id);
        } else {
            ++session_id;
        }
    }
}

std::shared_ptr<Session>
User::getSession(unsigned int session_id) const {
    if (activeSessions.rlock()->count(session_id) > 0)
        return server->getSession(session_id);
    return nullptr;
}

std::string User::getId() const {
    return id.copy();
}

} //restbes
