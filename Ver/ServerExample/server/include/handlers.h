#ifndef RESTAURANTBES_HANDLERS_H
#define RESTAURANTBES_HANDLERS_H

#include "server.h"

// TODO: не надо засорять общий неймспейс. Оберни в namespace проекта.
using server_structure::Session;
using server_structure::Server;

void getMethodHandler(std::shared_ptr<Session> session,
                      std::shared_ptr<Server> server);

void postEchoMethodHandler(std::shared_ptr<Session> session,
                           const std::string &data, std::shared_ptr<Server> server);


void postMessengerMethodHandler(std::shared_ptr<Session> session,
                                const std::string &data,
                                std::shared_ptr<Server> server);

void errorHandler(const int code,
                  const std::exception &exception,
                  std::shared_ptr<Session> session,
                  std::shared_ptr<Server> server);

void handleInactiveSessions(std::shared_ptr<Server> server);


#endif //RESTAURANTBES_HANDLERS_H
