#pragma once

#include "server.h"

using restbes::session_structure::Session;
using restbes::server_structure::Server;

void getMethodHandler(std::shared_ptr<Session> session,
                      std::shared_ptr<Server> server);

void postEchoMethodHandler(std::shared_ptr<restbed::Session> session,
                           const std::string &data, std::shared_ptr<Server> server);


void postMessengerMethodHandler(std::shared_ptr<restbed::Session> session,
                                const std::string &data,
                                std::shared_ptr<Server> server);

void errorHandler(const int code,
                  const std::exception &exception,
                  std::shared_ptr<restbed::Session> session,
                  std::shared_ptr<Server> server);

void handleInactiveSessions(std::shared_ptr<Server> server);
