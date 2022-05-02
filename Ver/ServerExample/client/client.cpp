#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "httplib.h"
#include <gflags/gflags.h>

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std::chrono_literals;

static bool ValidatePort(const char *flagname, gflags::int32 value) {
    if (value > 0 && value < 32768) {
        return true;
    }
    printf("Invalid value for --%s: %d\n", flagname, (int) value);
    return false;
}

static bool ValidateResource(const char *flagname, const std::string &value) {
    if (value.empty()) {
        printf("Invalid value for --%s: %s\n", flagname, value.c_str());
        return false;
    }
    return true;
}

DEFINE_string(name, "", "Username");
DEFINE_int32(port, 0, "What port to listen on");
DEFINE_string(server, "localhost", "Domain to send requests to");
DEFINE_string(resource, "", "Path to the resource to send requests to");
DEFINE_bool(count_seconds, false, "Send automated messages counting seconds");
DEFINE_bool(silent, false, "Don't receive incoming messages");

DEFINE_validator(port, &ValidatePort);
DEFINE_validator(resource, &ValidateResource);

int main(int argc, char **argv) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    std::string client_name = fLS::FLAGS_name;
    std::string address = "https://" + fLS::FLAGS_server + ':' +
                          std::to_string(fLI::FLAGS_port);

    auto pollingClient = std::make_shared<httplib::Client>(address);
    pollingClient->enable_server_certificate_verification(false);
    pollingClient->set_keep_alive(true);
    pollingClient->set_read_timeout(180);
    unsigned int session_id = 0;

    while (session_id == 0) {
        auto res = pollingClient->Get("/get");
        if (res == nullptr) {
            std::cout << "No response\n";
        } else if (res->status != 200) {
            std::cout << "Bad response " << res->status << '\n';
        } else session_id = std::stoi(res->get_header_value("Session-ID"));
    }
    std::cout << session_id << std::endl;

    httplib::Headers headers = {
            {"User-ID",    client_name},
            {"Session-ID", std::to_string(session_id)}
    };

    if (!fLB::FLAGS_silent) {
        std::thread([&pollingClient, &headers]() {
            while (true) {
                auto res = pollingClient->Get("/get", headers);
                if (res == nullptr) {
                    std::cout << "No response\n";
                    return 1;
                }
                if (res->status != 200) {
                    std::cout << "Bad response " << res->status << '\n';
                    continue;
                }
                std::cout << res->body;
                std::cout.flush();
            }
        }).detach();
    }

    auto postingClient = std::make_shared<httplib::Client>(address);
    postingClient->set_read_timeout(180);
    postingClient->enable_server_certificate_verification(false);
    std::string message;
    for (int i = 0; true; ++i) {
        if (fLB::FLAGS_count_seconds) {
            message = std::to_string(i);
            std::this_thread::sleep_for(1s);
        } else {
            std::getline(std::cin, message);
        }
        auto res = postingClient->Post(fLS::FLAGS_resource.c_str(), headers,
                                       message, "text/plain");
        if (res == nullptr) {
            std::cerr << "No response\n";
            return 1;
        } else if (res->status != 200) {
            std::cerr << "Bad response " << res->status << '\n';
        }
    }
}
