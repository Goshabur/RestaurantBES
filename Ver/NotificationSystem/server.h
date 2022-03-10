#ifndef RESTAURANTBES_SERVER_H
#define RESTAURANTBES_SERVER_H

#include <restbed>

using restbed::Session, restbed::Response;

std::shared_ptr<Response> generate_response(const std::string &body, bool keep_alive = false);

#endif //RESTAURANTBES_SERVER_H
