#include "server.h"
#include "chat.h"
#include "message.h"

using restbed::Session, restbed::Resource, restbed::Response, restbed::SSLSettings, restbed::Service, restbed::Uri, restbed::Settings, restbed::Bytes;

std::shared_ptr<Chat> &getChat() {
    static std::shared_ptr<Chat> chat(new Chat());
    return chat;
}

std::shared_ptr<Response> generateResponse(const std::string &body, Connection connection) {
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
    // make OK const
    response->set_status_message("OK");
    return response;
}

void getMethodHandler(std::shared_ptr<Session> session) {
    std::string name = session->get_request()->get_header("Name");
    if (getChat()->getUser(name) == nullptr) {
        getChat()->addUser(name, session);
    }
    auto user = getChat()->getUser(name);
    if (user->getSession(session) == nullptr) {
        user->addSession(session);
    }
}

HTTP_Handler generatePostMethodHandler(const std::function<void(std::shared_ptr<Session>, const std::string&)> &callback) {
    return [&callback](std::shared_ptr<Session> session) {
        int content_length = session->get_request()->get_header("Content-Length", 0);
        session->fetch(content_length, [callback](const std::shared_ptr<Session> session, const Bytes &body) {
            std::string data = std::string(body.begin(), body.end());
            callback(session, data);
        });
    };
}

void postMethodHandler(std::shared_ptr<Session> session, const std::string &data) {
        auto request = session->get_request();

        std::string name = request->get_header("Name", "Anonymous");
        getChat()->getUser(name)->sendMessage(data, getChat());
        // make const, send JSONs
        std::string response_body = "PROCESSED, OK\n";
        auto response = generateResponse(response_body, Connection::CLOSE);
        session->close(*response);
}

void errorHandler(const int code,
                  const std::exception &exception,
                  std::shared_ptr<Session> session) {}

void handleInactiveSessions() {
    for (const auto &user : getChat()->getUsers()) {
        user.second->eraseInactiveSessions();
    }
}

std::shared_ptr<Resource> createResource(const std::string &path,
                                         const HTTP_Handler &getMethodHandler,
                                         const HTTP_Handler &postMethodHandler,
                                         const ErrorHandler &errorHandler) {
    auto resource = std::make_shared<Resource>();
    resource->set_path(path);
    resource->set_method_handler("GET", getMethodHandler);
    resource->set_method_handler("POST", postMethodHandler);
    resource->set_error_handler(errorHandler);
    return resource;
}

std::shared_ptr<Settings> createSettingsWithSSL(const std::string &SSL_ServerKey,
                                         const std::string &SSL_Certificate,
                                         const std::string &SSL_DHKey,
                                         const int port,
                                         const int workers) {
    auto ssl_settings = std::make_shared<SSLSettings>();
    ssl_settings->set_http_disabled(true);
    ssl_settings->set_private_key(
            Uri("file://" + SSL_ServerKey));
    ssl_settings->set_certificate(
            Uri("file://" + SSL_Certificate));
    ssl_settings->set_temporary_diffie_hellman(
            Uri("file://" + SSL_DHKey));
    ssl_settings->set_port(port);
    auto settings = std::make_shared<Settings>();
    settings->set_ssl_settings(ssl_settings);
    settings->set_worker_limit(workers);
    return settings;
}