#pragma once

#include "server.h"

using restbes::Server;

void pollingHandler(std::shared_ptr<restbed::Session> session,
                    std::shared_ptr<Server> server);

void postEchoMethodHandler(std::shared_ptr<restbed::Session> session,
                           const std::string &data,
                           std::shared_ptr<Server> server);


void postMessengerMethodHandler(std::shared_ptr<restbed::Session> session,
                                const std::string &data,
                                std::shared_ptr<Server> server);

void errorHandler(int code,
                  const std::exception &exception,
                  std::shared_ptr<restbed::Session> session,
                  std::shared_ptr<Server> server);

void handleInactiveSessions(std::shared_ptr<Server> server);

void cleanUpUserSessions(std::shared_ptr<Server> server);
