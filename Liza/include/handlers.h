#pragma once

#include "server.h"

using restbes::Server;

namespace restbes {
std::shared_ptr<Server> &getServer();

void getMenuHandler(const std::shared_ptr<restbed::Session> &session,
                    const std::shared_ptr<Server> &server);

void getOrderHandler(const std::shared_ptr<restbed::Session> &session,
                     const std::shared_ptr<Server> &server);

void getCartHandler(const std::shared_ptr<restbed::Session> &session,
                    const std::shared_ptr<Server> &server);

void postAuthorizationMethodHandler(
    const std::shared_ptr<restbed::Session> &session,
    const std::string &data,
    const std::shared_ptr<Server> &server);

void postCartMethodHandler(const std::shared_ptr<restbed::Session> &session,
                           const std::string &data,
                           const std::shared_ptr<Server> &server);

void postOrderMethodHandler(const std::shared_ptr<restbed::Session> &session,
                            const std::string &data,
                            const std::shared_ptr<Server> &server);

void pollingHandler(const std::shared_ptr<restbed::Session>& session,
                    const std::shared_ptr<Server>& server);

void errorHandler(const int code,
                  const std::exception &exception,
                  const std::shared_ptr<restbed::Session>& session,
                  const std::shared_ptr<Server>& server);

void handleInactiveSessions(const std::shared_ptr<Server>& server);

void cleanUpUserSessions(const std::shared_ptr<Server>& server);

void notifySessionsMenuChanged();

void notifySessionsOrderChanged(const std::string &order_id);

std::string show_menu();

}  // namespace restbes
