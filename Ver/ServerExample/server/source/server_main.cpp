#include "server.h"
#include "handlers.h"
#include "restbed"
#include <gflags/gflags.h>
#include <filesystem>

using namespace std::chrono_literals;
using server_structure::createSettingsWithSSL;

static bool ValidatePath(const char *flagname, const std::string &value) {
    bool key = std::filesystem::exists(value + "/server.key");
    bool certificate = std::filesystem::exists(value + "/server.crt");
    bool diffie_hellman = std::filesystem::exists(value + "/dh2048.pem");
    if (key && certificate && diffie_hellman) {
        return true;
    }
    printf("Invalid value for --%s: "
           "can't find some of the required files: "
           "\"server.key\", \"server.crt\", \"dh2048.pem\"\n",
           flagname);
    return false;
}

DEFINE_string(SSLkeys, "", "Path to SSL keys");
DEFINE_validator(SSLkeys, &ValidatePath);

static bool ValidatePort(const char *flagname, gflags::int32 value) {
    if (0 < value && value < 32768) {
        return true;
    }
    printf("Invalid value for --%s: %d\n", flagname, (int) value);
    return false;
}

DEFINE_int32(port, 0, "What port to listen on");
DEFINE_validator(port, &ValidatePort);

static bool ValidateWorkers(const char *flagname, gflags::int32 value) {
    if (0 < value && value < 100) {
        return true;
    }
    printf("Invalid value for --%s: %d\n", flagname, (int) value);
    return false;
}

DEFINE_int32(workers, 10, "Number of workers");
DEFINE_validator(workers, &ValidateWorkers);

int main(int argc, char **argv) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    auto server = std::make_shared<Server>();

    auto resource = createResource("/messenger",
                                   getMethodHandler,
                                   postMethodHandler,
                                   errorHandler,
                                   server);

    std::string pathToSSL[] = {fLS::FLAGS_SSLkeys + "/server.key",
                               fLS::FLAGS_SSLkeys + "/server.crt",
                               fLS::FLAGS_SSLkeys + "/dh2048.pem"};
    auto settings = createSettingsWithSSL(pathToSSL[0],
                                          pathToSSL[1],
                                          pathToSSL[2],
                                          fLI::FLAGS_port,
                                          fLI::FLAGS_workers);

    server->addResource(resource);
    server->schedule(handleInactiveSessions, server, 1s);
    server->setSettings(settings);
    server->startServer();

    return EXIT_SUCCESS;
}
