#ifndef RESTAURANTBES_SERVER_H
#define RESTAURANTBES_SERVER_H

#include <restbed>
#include "fwd.h"
#include <shared_mutex>

using restbed::Session;
using restbed::Response;
using restbed::Resource;
using restbed::Settings;
using restbed::Service;
using HTTP_Handler = std::function<void(std::shared_ptr<Session>)>;
using GET_Handler = std::function<void(std::shared_ptr<Session>, std::shared_ptr<Server> server)>;
using POST_Handler = std::function<void(std::shared_ptr<Session>, const std::string &, std::shared_ptr<Server> server)>;
using restbed_ErrorHandler = std::function<void(const int, const std::exception &, std::shared_ptr<Session>)>;
using ErrorHandler = std::function<void(const int, const std::exception &, std::shared_ptr<Session>, std::shared_ptr<Server>)>;
using ScheduledTask = std::function<void(std::shared_ptr<Server> server)>;
using UserCollection = std::unordered_map<std::string, std::shared_ptr<User>>;

enum ResponseCode {
  OK = 200
};

struct Server {
private:
    UserCollection users;
    mutable std::shared_mutex mutexUsers;

    std::shared_ptr<Settings> settings;
    std::shared_ptr<Service> service;

public:
    Server();
    [[nodiscard]] UserCollection getUsers() const;
    [[nodiscard]] std::shared_ptr<User> getUser(const std::string &name) const;
    void addUser(const std::string &name, std::shared_ptr<Session> session);
    void addResource(std::shared_ptr<Resource> resource);
    void setSettings(std::shared_ptr<Settings> newSettings);
    void schedule(const ScheduledTask &task,
                  std::shared_ptr<Server> server,
                  const std::chrono::duration<int64_t, std::ratio<1, 1000>> &interval = std::chrono::milliseconds::zero());
    void startServer();
};

[[nodiscard]] std::shared_ptr<Response> generateResponse(const std::string &body, Connection connection = Connection::CLOSE);

[[nodiscard]] HTTP_Handler generatePostMethodHandler(const POST_Handler &callback, std::shared_ptr<Server> server);

[[nodiscard]] HTTP_Handler generateGetMethodHandler(const GET_Handler &callback, std::shared_ptr<Server> server);

[[nodiscard]] std::function<void(void)> generateScheduledTask(const ScheduledTask &task, std::shared_ptr<Server> server);

[[nodiscard]] restbed_ErrorHandler generateErrorHandler(const ErrorHandler &callback, std::shared_ptr<Server> server);

std::shared_ptr<Resource> createResource(const std::string &path,
                                         const GET_Handler &getMethodHandler,
                                         const POST_Handler &postMethodHandler,
                                         const ErrorHandler &errorHandler,
                                         std::shared_ptr<Server> server);

std::shared_ptr<Settings> createSettingsWithSSL(const std::string &SSL_ServerKey,
                                                const std::string &SSL_Certificate,
                                                const std::string &SSL_DHKey,
                                                const int port,
                                                const int workers);

#endif //RESTAURANTBES_SERVER_H
