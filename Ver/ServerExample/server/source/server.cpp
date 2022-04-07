#include "server.h"
#include "user.h"

#include <utility>

namespace restbes::server_structure {

Server::Server() : service(new restbed::Service()) {}

Server::UserCollection Server::getUsers() const {
    return *(users.rlock());
}

std::shared_ptr<User> Server::getUser(const std::string &name) const {
    if (users.rlock()->count(name) == 0) return nullptr;
    else return users.rlock()->at(name);
}

void Server::addUser(const std::string &name) {
    if (users.rlock()->count(name) == 0) {
        users.wlock()->insert({name, std::make_shared<User>(name)});
    }
}

void Server::addResource(std::shared_ptr<restbed::Resource> resource) {
    service->publish(resource);
}

void Server::setSettings(std::shared_ptr<restbed::Settings> newSettings) {
    settings = std::move(newSettings);
}

void Server::schedule(const ScheduledTask &task,
                      std::shared_ptr<Server> server,
                      const std::chrono::duration<int64_t, std::ratio<1, 1000>> &interval) {
    service->schedule(generateScheduledTask(task, std::move(server)), interval);
}

void Server::startServer() {
    service->start(settings);
}

std::shared_ptr<restbed::Response>
generateResponse(const std::string &body, const std::string &content_type, Connection connection) {
    auto response = std::make_shared<restbed::Response>();
    response->set_body(body);
    response->set_header("Content-Length", std::to_string(body.size()));
    response->set_header("Content-Type", content_type);
    switch (connection) {
        case Connection::KEEP_ALIVE:
            response->set_header("Connection", "keep-alive");
            break;
        case Connection::CLOSE:
            response->set_header("Connection", "close");
            break;
    }
    response->set_status_code(ResponseCode::OK);
    // TODO: make OK const
    response->set_status_message("OK");
    return response;
}

restbed_HTTP_Handler
Server::generatePostMethodHandler(const POST_Handler &callback,
                                  std::shared_ptr<Server> server) {
    return [callback, server](std::shared_ptr<Session> session) {
        int content_length = session->get_request()->get_header(
                "Content-Length", 0);
        session->fetch(
                content_length,
                [callback, server](
                        const std::shared_ptr<Session> session,
                        const restbed::Bytes &body) {
                    std::string data = std::string(body.begin(), body.end());
                    callback(session, data, server);
                });
    };
}

restbed_HTTP_Handler
Server::generateGetMethodHandler(const GET_Handler &callback,
                                 std::shared_ptr<Server> server) {
    return [callback, server](std::shared_ptr<Session> session) {
        callback(std::move(session), server);
    };
}

std::function<void(void)>
Server::generateScheduledTask(const ScheduledTask &task,
                              std::shared_ptr<Server> server) {
    return [task, server]() {
        task(server);
    };
}

restbed_ErrorHandler Server::generateErrorHandler(const ErrorHandler &callback,
                                                  std::shared_ptr<Server> server) {
    return [callback, server](const int code,
                              const std::exception &exception,
                              std::shared_ptr<Session> session) {
        callback(code, exception, std::move(session), server);
    };
}

std::shared_ptr<restbed::Resource> createResource(const std::string &path,
                                         const Server::GET_Handler &getMethodHandler,
                                         const Server::POST_Handler &postMethodHandler,
                                         const Server::ErrorHandler &errorHandler,
                                         std::shared_ptr<Server> server) {
    auto resource = std::make_shared<restbed::Resource>();
    resource->set_path(path);
    resource->set_method_handler("GET",
                                 Server::generateGetMethodHandler(
                                         getMethodHandler,
                                         server));
    resource->set_method_handler("POST",
                                 Server::generatePostMethodHandler(
                                         postMethodHandler,
                                         server));
    resource->set_error_handler(
            Server::generateErrorHandler(errorHandler, server));
    return resource;
}

std::shared_ptr<restbed::Settings> createSettingsWithSSL(
        const std::string &SSL_ServerKey,
        const std::string &SSL_Certificate,
        const std::string &SSL_DHKey,
        const int port,
        const int workers) {
    auto ssl_settings = std::make_shared<restbed::SSLSettings>();
    ssl_settings->set_http_disabled(true);
    ssl_settings->set_private_key(
            restbed::Uri("file://" + SSL_ServerKey));
    ssl_settings->set_certificate(
            restbed::Uri("file://" + SSL_Certificate));
    ssl_settings->set_temporary_diffie_hellman(
            restbed::Uri("file://" + SSL_DHKey));
    ssl_settings->set_port(port);
    auto settings = std::make_shared<restbed::Settings>();
    settings->set_ssl_settings(ssl_settings);
    settings->set_worker_limit(workers);
    return settings;
}

} //server_structure