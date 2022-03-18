#include "user.h"
#include "server.h"

User::User(std::string nm, std::shared_ptr<Session> session) : name(std::move(nm)), activeSessions({std::move(session)}) {}

//void User::sendMessage(const std::string &text, std::shared_ptr<Chat> chat) const {
//    chat->addMessage(Message{name, text});
//}

void User::yieldMessage(const std::string &message) {
    auto response = generateResponse(message, Connection::KEEP_ALIVE);
    std::unique_lock lock(mutex);
    for (const auto &session : activeSessions) {
        if (session->is_open()) {
            session->yield(*response);
        }
    }
}

void User::addSession(std::shared_ptr<Session> session) {
    std::unique_lock lock(mutex);
    activeSessions.insert(std::move(session));
}

void User::eraseInactiveSessions() {
    std::unique_lock lock(mutex);
    for (auto session = activeSessions.begin(); session != activeSessions.end(); ) {
        if ((*session)->is_closed()) {
            session = activeSessions.erase(session);
        } else {
            ++session;
        }
    }
}

std::shared_ptr<Session> User::getSession(std::shared_ptr<Session> session) const {
    std::shared_lock lock(mutex);
    if (activeSessions.count(session) == 0) return nullptr;
    else return session;
}

const std::unordered_set<std::shared_ptr<Session>> &User::getActiveSessions() const {
    std::shared_lock lock(mutex);
    return activeSessions;
}

const std::string &User::getName() const {
    std::shared_lock lock(mutex);
    return name;
}
