#include "server.h"
#include "restbed"
#include <gflags/gflags.h>
#include <filesystem>

using restbed::Session, restbed::Resource, restbed::SSLSettings, restbed::Service, restbed::Uri, restbed::Settings, restbed::Bytes;
using namespace std::chrono_literals;

static bool ValidatePath(const char *flagname, const std::string &value) {
    bool key = std::filesystem::exists(value + "/server.key");
    bool certificate = std::filesystem::exists(value + "/server.crt");
    bool diffie_hellman = std::filesystem::exists(value + "/dh2048.pem");
    if (key && certificate && diffie_hellman) {
        return true;
    }
    printf("Invalid value for --%s: can't find some of the required files: \"server.key\", \"server.crt\", \"dh2048.pem\"\n",
           flagname);
    return false;
}

DEFINE_string(SSL_keys, "", "Path to SSL keys");
DEFINE_validator(SSL_keys, &ValidatePath);

static bool ValidatePort(const char *flagname, gflags::int32 value) {
    if (value > 0 && value < 32768) {
        return true;
    }
    printf("Invalid value for --%s: %d\n", flagname, (int) value);
    return false;
}

DEFINE_int32(port, 0, "What port to listen on");
DEFINE_validator(port, &ValidatePort);

static bool ValidateWorkers(const char *flagname, gflags::int32 value) {
    if (value > 0 && value < 100) {
        return true;
    }
    printf("Invalid value for --%s: %d\n", flagname, (int) value);
    return false;
}

DEFINE_int32(workers, 10, "Number of workers");
DEFINE_validator(workers, &ValidateWorkers);

int main(int argc, char **argv) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    auto resource = std::make_shared<Resource>();
    resource->set_path("/messenger");
    resource->set_method_handler("GET", get_method_handler);
    resource->set_method_handler("POST", post_method_handler);

    auto ssl_settings = std::make_shared<SSLSettings>();
    ssl_settings->set_http_disabled(true);
    ssl_settings->set_private_key(
            Uri("file://" + fLS::FLAGS_SSL_keys + "/server.key"));
    ssl_settings->set_certificate(
            Uri("file://" + fLS::FLAGS_SSL_keys + "/server.crt"));
    ssl_settings->set_temporary_diffie_hellman(
            Uri("file://" + fLS::FLAGS_SSL_keys + "/dh2048.pem"));
    ssl_settings->set_port(fLI::FLAGS_port);

    auto settings = std::make_shared<Settings>();
    settings->set_ssl_settings(ssl_settings);
    settings->set_worker_limit(fLI::FLAGS_workers);

    Service service;
    service.publish(resource);
    service.schedule(handle_inactive_sessions, 1s);
    service.start(settings);

    return EXIT_SUCCESS;
}
