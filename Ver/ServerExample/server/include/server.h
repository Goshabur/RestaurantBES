#pragma once

#include "fwd.h"

#include <restbed>

#include <shared_mutex>
#include <memory>

namespace restbes::server_structure {

using restbed::Session;
using user_structure::User;
using restbed_HTTP_Handler = std::function<void(std::shared_ptr<Session>)>;
using restbed_ErrorHandler = std::function<void(const int,
                                                const std::exception &,
                                                std::shared_ptr<Session>)>;

enum ResponseCode {
    OK = 200
};

struct Server {
    using GET_Handler = std::function<void(std::shared_ptr<Session>,
                                           std::shared_ptr<Server> server)>;
    using POST_Handler = std::function<void(std::shared_ptr<Session>,
                                            const std::string &,
                                            std::shared_ptr<Server> server)>;
    using ErrorHandler = std::function<void(const int, const std::exception &,
                                            std::shared_ptr<Session>,
                                            std::shared_ptr<Server>)>;
    using ScheduledTask = std::function<void(std::shared_ptr<Server> server)>;
    using UserCollection = std::unordered_map<std::string, std::shared_ptr<User>>;

private:
    UserCollection users;
    mutable std::shared_mutex mutexUsers;

    std::shared_ptr<restbed::Settings> settings;
    std::shared_ptr<restbed::Service> service;

    [[nodiscard]] static restbed_HTTP_Handler
    generatePostMethodHandler(const POST_Handler &callback,
                              std::shared_ptr<Server> server);

    [[nodiscard]] static restbed_HTTP_Handler
    generateGetMethodHandler(const GET_Handler &callback,
                             std::shared_ptr<Server> server);

    [[nodiscard]] static std::function<void(void)>
    generateScheduledTask(const ScheduledTask &task,
                          std::shared_ptr<Server> server);

    [[nodiscard]] static restbed_ErrorHandler
    generateErrorHandler(const ErrorHandler &callback,
                         std::shared_ptr<Server> server);

    friend std::shared_ptr<restbed::Resource> createResource(const std::string &path,
                                                    const GET_Handler &getMethodHandler,
                                                    const POST_Handler &postMethodHandler,
                                                    const ErrorHandler &errorHandler,
                                                    std::shared_ptr<Server> server);

public:
    Server();

    [[nodiscard]] UserCollection getUsers() const;

    [[nodiscard]] std::shared_ptr<User>
    getUser(const std::string &name) const;

    void addUser(const std::string &name);

    void addResource(std::shared_ptr<restbed::Resource> resource);

    void setSettings(std::shared_ptr<restbed::Settings> newSettings);

    void schedule(const ScheduledTask &task,
                  std::shared_ptr<Server> server,
                  const std::chrono::duration<int64_t, std::ratio<1, 1000>> &interval = std::chrono::milliseconds::zero());

    void startServer();
};

[[nodiscard]] std::shared_ptr<restbed::Response>
generateResponse(const std::string &body,
                 const std::string &content_type,
                 Connection connection = Connection::CLOSE);

std::shared_ptr<restbed::Resource> createResource(const std::string &path,
                                         const Server::GET_Handler &getMethodHandler,
                                         const Server::POST_Handler &postMethodHandler,
                                         const Server::ErrorHandler &errorHandler,
                                         std::shared_ptr<Server> server);

std::shared_ptr<restbed::Settings>
createSettingsWithSSL(const std::string &SSL_ServerKey,
                      const std::string &SSL_Certificate,
                      const std::string &SSL_DHKey,
                      const int port,
                      const int workers);

} //server_structure
