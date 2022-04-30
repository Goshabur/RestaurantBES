#include "session.h"
#include "server.h"

#include <string>

namespace restbes::session_structure {

Session::Session(std::shared_ptr<restbed::Session> ss, std::string uid) : session(std::move(ss)), user_id(std::move(uid)) {
//        if (session_id.copy() == 0) {
//            *(session_id.wlock()) = ++counter;
//            auto response = server_structure::generateResponse("Session-ID: " + std::to_string(session_id.copy()), "text/plain", server_structure::Connection::KEEP_ALIVE);
//            session->yield(*response);
//        }
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
    while (!responseQueue.rlock()->empty() && is_open()) {
        session->yield(*(responseQueue.rlock()->front()));
        responseQueue.wlock()->pop();
    }
}

std::string Session::getUserId() const {
    return user_id.copy();
}

std::shared_ptr<restbed::Session> Session::getSession() const {
    return session;
}

//[[nodiscard]] unsigned int Session::getId() const {
//    return session_id.copy();
//}

}
