#ifndef RESTAURANTBES_SERVER_H
#define RESTAURANTBES_SERVER_H

#include <corvusoft/restbed/session.hpp>
#include "fwd.h"

using restbed::Session, restbed::Response;

enum ResponseCode {
  OK = 200
};

std::shared_ptr<Chat> &getChat();

std::shared_ptr<Response> generate_response(const std::string &body, Connection connection = Connection::CLOSE);

void get_method_handler(std::shared_ptr<Session> session);
void post_method_handler(std::shared_ptr<Session> session);
void handle_inactive_sessions();

#endif //RESTAURANTBES_SERVER_H
