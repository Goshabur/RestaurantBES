#include <cstdlib>
#include <restbed>
#include <unordered_map>
#include <shared_mutex>
#include "chat.h"

using namespace restbed;

std::unordered_map<std::string, Chat> chat_collection;
std::shared_mutex mu_collection;

std::shared_ptr< Response > generate_response(const std::string &body, bool keep_alive = false)
{
    auto response = std::make_shared< Response >();
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

void get_method_handler(std::shared_ptr<Session> session) {
    static int id = 0;
    if (session->get_id() == "") {
        session->set_id(std::to_string(++id));
        std::unique_lock<std::shared_mutex> lock(mu_collection);
        chat_collection.insert({session->get_id(), {}});
    }
    std::shared_lock<std::shared_mutex> sharedLock(mu_collection);
    std::string messages = chat_collection.at(session->get_id()).receive();
    sharedLock.unlock();
    auto response = generate_response(messages, true);
    session->yield(*response);
}

void post_method_handler(std::shared_ptr<Session> session) {
    const auto request = session->get_request( );

    int content_length = request->get_header("Content-Length", 0);
    session->fetch( content_length, []( const std::shared_ptr< Session > session, const Bytes & body )
    {
        std::string data;
        for (auto ch : body) {
            data += static_cast<char>(ch);
        }
        std::string response_body = "I've processed your POST request\n";
        Chat::send(data);

        auto response = generate_response(response_body);
        session->close( *response );
    } );
}

int main( const int, const char** )
{
    auto resource = std::make_shared<Resource>();
    resource->set_path("/messenger");
    resource->set_method_handler( "GET", get_method_handler );
    resource->set_method_handler( "POST", post_method_handler );

    auto ssl_settings = std::make_shared< SSLSettings >( );
    ssl_settings->set_http_disabled( true );
    ssl_settings->set_private_key( Uri( "file:///home/veronika/hse_study/RestaurantBES/Ver/Messenger/SSL-keys/server.key" ) );
    ssl_settings->set_certificate( Uri( "file:///home/veronika/hse_study/RestaurantBES/Ver/Messenger/SSL-keys/server.crt" ) );
    ssl_settings->set_temporary_diffie_hellman( Uri( "file:///home/veronika/hse_study/RestaurantBES/Ver/Messenger/SSL-keys/dh2048.pem" ) );
    ssl_settings->set_port(1234);

    auto settings = std::make_shared< Settings >( );
    settings->set_ssl_settings( ssl_settings );
    settings->set_worker_limit(100);

    Service service;
    service.publish( resource );
    service.start( settings );

    return EXIT_SUCCESS;
}