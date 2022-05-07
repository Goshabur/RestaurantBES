#pragma once

#include "fwd.h"

#include <restbed>
#include <folly/Synchronized.h>

#include <memory>

namespace restbes {

using restbed_HTTP_Handler = std::function<void(
        std::shared_ptr<restbed::Session>)>;
using restbed_ErrorHandler = std::function<void(const int,
                                                const std::exception &,
                                                std::shared_ptr<restbed::Session>)>;

enum ResponseCode {
    OK = 200
};

struct Server {
    using GET_Handler = std::function<void(
            std::shared_ptr<restbed::Session>,
            std::shared_ptr<Server> server)>;
    using POST_Handler = std::function<void(std::shared_ptr<restbed::Session>,
                                            const std::string &,
                                            std::shared_ptr<Server> server)>;
    using ErrorHandler = std::function<void(const int, const std::exception &,
                                            std::shared_ptr<restbed::Session>,
                                            std::shared_ptr<Server>)>;
    using ScheduledTask = std::function<void(std::shared_ptr<Server> server)>;
    using UserCollection = std::unordered_map<std::string, std::shared_ptr<User>>;
    using SessionCollection = std::unordered_map<unsigned int, std::shared_ptr<Session>>;

private:
    folly::Synchronized<UserCollection> users;
    folly::Synchronized<SessionCollection> sessions;

    std::shared_ptr<restbed::Settings> settings;
    std::shared_ptr<restbed::Service> service;

    inline static std::atomic<unsigned int> sessionCounter{0};

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

    friend std::shared_ptr<restbed::Resource>
    createResource(const std::string &path,
                   const std::optional<GET_Handler> &getMethodHandler,
                   const std::optional<POST_Handler> &postMethodHandler,
                   const ErrorHandler &errorHandler,
                   std::shared_ptr<Server> server);

public:
    Server();

    [[nodiscard]] folly::Synchronized<UserCollection>::ConstRLockedPtr
    getUsers() const;

    folly::Synchronized<UserCollection>::WLockedPtr getUsersW();

    [[nodiscard]] folly::Synchronized<SessionCollection>::ConstRLockedPtr
    getSessions() const;

    [[nodiscard]] folly::Synchronized<SessionCollection>::WLockedPtr
    getSessionsW();

    [[nodiscard]] std::shared_ptr<Session>
    getSession(unsigned int session_id) const;

    unsigned int
    addSession(std::shared_ptr<restbed::Session> session, std::string user_id);

    void
    assignSession(unsigned int session_id, const std::string &user_id) const;

    [[nodiscard]] std::shared_ptr<User> getUser(const std::string &name) const;

    static void addUser(const std::string &name, std::shared_ptr<Server> serv);

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

std::shared_ptr<restbed::Settings>
createSettingsWithSSL(const std::string &SSL_ServerKey,
                      const std::string &SSL_Certificate,
                      const std::string &SSL_DHKey,
                      int port,
                      int workers);

} //restbes
