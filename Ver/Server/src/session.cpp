#include "session.h"
#include "server.h"

#include <string>

namespace restbes {

void Session::yieldFromQueue() {
    while (!responseQueue.rlock()->empty() && is_open()) {
        session->yield(*(responseQueue.rlock()->front()));
        responseQueue.wlock()->pop();
    }
}

Session::Session(std::shared_ptr<restbed::Session> ss, std::string uid)
        : session(std::move(ss)), user_id(std::move(uid)) {
}

void Session::setUser(std::string uid) {
    *(user_id.wlock()) = std::move(uid);
}

void Session::setSession(std::shared_ptr<restbed::Session> ss) {
    session = std::move(ss);
    yieldFromQueue();
}

bool Session::is_open() const {
    return session->is_open();
}

bool Session::is_closed() const {
    return session->is_closed();
}

std::string Session::getPath() const {
    return session->get_request()->get_path();
}

void Session::push(std::shared_ptr<restbed::Response> response) {
    responseQueue.wlock()->push(response);
    yieldFromQueue();
}

std::string Session::getUserId() const {
    return user_id.copy();
}

std::shared_ptr<restbed::Session> Session::getSession() const {
    return session;
}

[[nodiscard]] unsigned int Session::getId() const {
    return session_id.copy();
}

} //restbes
