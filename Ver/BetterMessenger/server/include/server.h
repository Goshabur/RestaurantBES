#ifndef RESTAURANTBES_SERVER_H
#define RESTAURANTBES_SERVER_H

#include <corvusoft/restbed/session.hpp>
#include <corvusoft/restbed/settings.hpp>
#include "fwd.h"

using restbed::Session, restbed::Response, restbed::Resource, restbed::Settings;
using HTTP_Handler = std::function<void(std::shared_ptr<Session>)>;
using ErrorHandler = std::function<void(const int, const std::exception &, std::shared_ptr<Session>)>;

enum ResponseCode {
  OK = 200
};

[[nodiscard]] std::shared_ptr<Chat> &getChat();

[[nodiscard]] std::shared_ptr<Response> generateResponse(const std::string &body, Connection connection = Connection::CLOSE);

void getMethodHandler(std::shared_ptr<Session> session);

HTTP_Handler generatePostMethodHandler(const std::function<void(std::shared_ptr<Session>, const std::string&)> &callback);
void postMethodHandler(std::shared_ptr<Session> session, const std::string &data);

void errorHandler(const int code,
                  const std::exception &exception,
                  std::shared_ptr<Session> session);

void handleInactiveSessions();

std::shared_ptr<Resource> createResource(const std::string &path,
                                         const HTTP_Handler &getMethodHandler,
                                         const HTTP_Handler &postMethodHandler,
                                         const ErrorHandler &errorHandler);

std::shared_ptr<Settings> createSettingsWithSSL(const std::string &SSL_ServerKey,
                                                const std::string &SSL_Certificate,
                                                const std::string &SSL_DHKey,
                                                const int port,
                                                const int workers);

#endif //RESTAURANTBES_SERVER_H
