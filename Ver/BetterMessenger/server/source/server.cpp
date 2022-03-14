#include "server.h"
#include "chat.h"
#include "message.h"
#include "user.h"

using restbed::Session, restbed::Resource, restbed::Response, restbed::SSLSettings, restbed::Service, restbed::Uri, restbed::Settings, restbed::Bytes;

std::shared_ptr<Chat> &getChat() {
    static std::shared_ptr<Chat> chat(new Chat());
    return chat;
}

std::shared_ptr<Response> generate_response(const std::string &body, Connection connection) {
    auto response = std::make_shared<Response>();
    response->set_body(body);
    response->set_header("Content-Length", std::to_string(body.size()));
    switch (connection) {
        case Connection::KEEP_ALIVE:
            response->set_header("Connection", "keep-alive");
            break;
        case Connection::CLOSE:
            response->set_header("Connection", "close");
            break;
    }
    response->set_status_code(ResponseCode::OK);
    response->set_status_message("OK");
    return response;
}

void get_method_handler(std::shared_ptr<Session> session) {
    std::string name = session->get_request()->get_header("Name");
    if (getChat()->getUser(name) == nullptr) {
        getChat()->addUser(name, session);
    }
    auto user = getChat()->getUser(name);
    if (user->getSession(session) == nullptr) {
        user->addSession(session);
    }
}

void post_method_handler(std::shared_ptr<Session> session) {
    const auto request = session->get_request();

    int content_length = request->get_header("Content-Length", 0);
    std::string name = request->get_header("Name", "Anonymous"); // some problems here
    session->fetch(content_length, [name](const std::shared_ptr<Session> session, const Bytes &body) {
        std::string data(body.begin(), body.end());

        getChat()->getUser(name)->sendMessage(data, getChat());

        // make a const, send JSONs
        std::string response_body = "PROCESSED, OK\n";
        auto response = generate_response(response_body, Connection::CLOSE);
        session->close(*response);
    });
}

void handle_inactive_sessions() {
    for (auto &user : getChat()->getUsers()) {
        user.second->eraseInactiveSessions();
    }
}
