#include "server.h"
#include <utility>
#include "user.h"

namespace server_structure {

using restbed::Bytes;
using restbed::SSLSettings;
using restbed::Uri;

Server::Server() : service(new Service()) {}

UserCollection Server::getUsers() const {
    std::shared_lock lock(mutexUsers);
    return users;
}

std::shared_ptr<User> Server::getUser(const std::string &name) const {
    std::shared_lock lock(mutexUsers);
    if (users.count(name) == 0) return nullptr;
    else return users.at(name);
}

void
Server::addUser(const std::string &name, std::shared_ptr<Session> session) {
    std::shared_lock lock(mutexUsers);
    if (users.count(name) == 0) {
        lock.unlock();
        std::unique_lock uLock(mutexUsers);
        users.insert({name, std::make_shared<User>(name, session)});
    }
}

void Server::addResource(std::shared_ptr<Resource> resource) {
    service->publish(resource);
}

void Server::setSettings(std::shared_ptr<Settings> newSettings) {
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

std::shared_ptr<Response>
generateResponse(const std::string &body, Connection connection) {
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
    // TODO: make OK const
    response->set_status_message("OK");
    return response;
}

restbed_HTTP_Handler Server::generatePostMethodHandler(const POST_Handler &callback,
                                       std::shared_ptr<Server> server) {
    return [callback, server](std::shared_ptr<Session> session) {
        int content_length = session->get_request()->get_header(
                "Content-Length", 0);
        session->fetch(
                content_length,
                [callback, server](
                        const std::shared_ptr<Session> session,
                        const Bytes &body) {
                    std::string data = std::string(body.begin(), body.end());
                    callback(session, data, server);
                });
    };
}

restbed_HTTP_Handler Server::generateGetMethodHandler(const GET_Handler &callback,
                                      std::shared_ptr<Server> server) {
    return [callback, server](std::shared_ptr<Session> session) {
        callback(std::move(session), server);
    };
}

std::function<void(void)> Server::generateScheduledTask(const ScheduledTask &task,
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

std::shared_ptr<Resource> createResource(const std::string &path,
                                         const GET_Handler &getMethodHandler,
                                         const POST_Handler &postMethodHandler,
                                         const ErrorHandler &errorHandler,
                                         std::shared_ptr<Server> server) {
    auto resource = std::make_shared<Resource>();
    resource->set_path(path);
    resource->set_method_handler("GET",
                                 Server::generateGetMethodHandler(getMethodHandler,
                                                          server));
    resource->set_method_handler("POST",
                                 Server::generatePostMethodHandler(postMethodHandler,
                                                           server));
    resource->set_error_handler(Server::generateErrorHandler(errorHandler, server));
    return resource;
}

std::shared_ptr<Settings> createSettingsWithSSL(
        const std::string &SSL_ServerKey,
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

} //server_structure