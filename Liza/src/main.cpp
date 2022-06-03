#include <gflags/gflags.h>
#include <filesystem>
#include "handlers.h"

using namespace std::chrono_literals;
using restbes::getServer;
using restbes::errorHandler;

static bool ValidatePath(const char *flagname, const std::string &value) {
    bool key = std::filesystem::exists(value + "/server.key");
    bool certificate = std::filesystem::exists(value + "/server.crt");
    bool diffie_hellman = std::filesystem::exists(value + "/dh2048.pem");
    if (key && certificate && diffie_hellman) {
        return true;
    }
    printf(
        "Invalid value for --%s: "
        "can't find some of the required files: "
        "\"server.key\", \"server.crt\", \"dh2048.pem\"\n",
        flagname);
    return false;
}

static bool ValidatePort(const char *flagname, gflags::int32 value) {
    if (0 < value && value < 32768) {
        return true;
    }
    printf("Invalid value for --%s: %d\n", flagname, (int)value);
    return false;
}

static bool ValidateWorkers(const char *flagname, gflags::int32 value) {
    if (0 < value && value < 100) {
        return true;
    }
    printf("Invalid value for --%s: %d\n", flagname, (int)value);
    return false;
}

DEFINE_string(SSLkeys, "", "Path to SSL keys");
DEFINE_int32(port, 0, "What port to listen on");
DEFINE_int32(workers, 10, "Number of workers");

DEFINE_validator(SSLkeys, &ValidatePath);
DEFINE_validator(port, &ValidatePort);
DEFINE_validator(workers, &ValidateWorkers);

int main(int argc, char **argv) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    auto order = createResource("/order", restbes::getOrderHandler,
                                restbes::postOrderMethodHandler, errorHandler,
                                getServer());

    auto cart = createResource("/cart", restbes::getCartHandler,
                               restbes::postCartMethodHandler, errorHandler,
                               getServer());

    auto menu = createResource("/menu", restbes::getMenuHandler, std::nullopt,
                               errorHandler, getServer());

    auto user = createResource("/user", std::nullopt,
                               restbes::postAuthorizationMethodHandler,
                               errorHandler, getServer());

    auto get = createResource("/get", restbes::pollingHandler, std::nullopt,
                              errorHandler, getServer());

    std::string pathToSSL[] = {fLS::FLAGS_SSLkeys + "/server.key",
                               fLS::FLAGS_SSLkeys + "/server.crt",
                               fLS::FLAGS_SSLkeys + "/dh2048.pem"};

    auto settings =
        restbes::createSettingsWithSSL(pathToSSL[0], pathToSSL[1], pathToSSL[2],
                                       fLI::FLAGS_port, fLI::FLAGS_workers);

    getServer()->addResource(order);
    getServer()->addResource(cart);
    getServer()->addResource(user);
    getServer()->addResource(get);
    getServer()->addResource(menu);
    getServer()->schedule(restbes::handleInactiveSessions, getServer(), 1s);
    getServer()->schedule(restbes::cleanUpUserSessions, getServer(), 2s);
    //    getServer()server->schedule(swapMenus, server, 3s);
    getServer()->setSettings(settings);
    getServer()->startServer();

    return EXIT_SUCCESS;
}
