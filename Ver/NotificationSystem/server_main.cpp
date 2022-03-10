#include <cstdlib>
#include <restbed>
#include <unordered_map>
#include <shared_mutex>
#include "chat.h"
#include "server.h"

using restbed::Session, restbed::Resource, restbed::SSLSettings, restbed::Service, restbed::Uri, restbed::Settings;

// folly synchronized; store ptrs, not values
std::unordered_map<std::string, User> user_collection;

void get_method_handler(std::shared_ptr<Session> session) {

}

void post_method_handler(std::shared_ptr<Session> session) {

}

int main( const int, const char** )
{
    auto resource = std::make_shared<Resource>();
    resource->set_path("/messenger");
    resource->set_method_handler( "GET", get_method_handler );
    resource->set_method_handler( "POST", post_method_handler );

    auto ssl_settings = std::make_shared< SSLSettings >( );
    ssl_settings->set_http_disabled( true );
    // gflags
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