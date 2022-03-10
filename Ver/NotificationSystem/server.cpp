#include "chat.h"
#include "server.h"

std::shared_ptr<Response> generate_response(const std::string &body, bool keep_alive)
{
    auto response = std::make_shared<Response>();
    response->set_body(body);
    response->set_header("Content-Length", std::to_string(body.size()));
    if (keep_alive) {
        response->set_header("Connection", "keep-alive");
    } else {
        response->set_header("Connection", "close");
    }
    response->set_status_code(200);
    response->set_status_message("OK");
    return response;
}