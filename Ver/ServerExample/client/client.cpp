#define CPPHTTPLIB_OPENSSL_SUPPORT

// nit: лучше разбить на блоки. 
// стандартные библиотеки
// внешние библиотеки
// местные хедеры

#include "httplib.h"
#include <iostream>
#include <thread>
#include <gflags/gflags.h>
#include <iostream>

using namespace std::chrono_literals;

DEFINE_string(name, "Anonymous", "Username");

static bool ValidatePort(const char *flagname, gflags::int32 value) {
    if (value > 0 && value < 32768) {
        return true;
    }
    printf("Invalid value for --%s: %d\n", flagname, (int) value);
    return false;
}

DEFINE_int32(port, 0, "What port to listen on");
DEFINE_validator(port, &ValidatePort);

DEFINE_string(server, "localhost", "Domain to send requests to");

static bool ValidateResource(const char *flagname, const std::string &value) {
    if (value.empty()) {
        printf("Invalid value for --%s: %s\n", flagname, value.c_str());
        return false;
    }
    return true;
}

DEFINE_string(resource, "", "Path to the resource to send requests to");
DEFINE_validator(resource, &ValidateResource);

DEFINE_bool(count_seconds, false, "Send automated messages counting seconds");

DEFINE_bool(silent, false, "Don't receive incoming messages");

int main(int argc, char **argv) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    std::string client_name = fLS::FLAGS_name;

    // TODO: Тут можно попробовать folly::sformat("http://{}:{}", server, port)
    std::string address = "https://" + fLS::FLAGS_server + ':' +
                          std::to_string(fLI::FLAGS_port);
    // make const
    auto cli = std::make_shared<httplib::Client>(address);
    cli->enable_server_certificate_verification(false);
    httplib::Headers headers = {
            {"Name", client_name}
    };

    if (!fLB::FLAGS_silent) {
        // TODO: по хорошему, лучше сохранять указатель тред. 
        // Перед завершением дожидаться завершения треда через join.
        // Внутрь добавить инструмент, чтобы завершить тред.
        std::thread([headers, address]() {
            // nit: cli обычно обозначает command-line interface. Лучше уж client
            auto cli2 = std::make_shared<httplib::Client>(address);
            cli2->enable_server_certificate_verification(false);
            cli2->set_keep_alive(true);
            cli2->set_read_timeout(180);

            while (true) {
                auto res = cli2->Get(fLS::FLAGS_resource.c_str(), headers);
                if (res == nullptr) {
                    // nit: Лучше LOG(WARNING) из glog
                    std::cout << "No response\n";
                    break;
                }
                if (res->status != 200) {
                    std::cout << "Bad response " << res->status << '\n';
                    continue;
                }
                std::cout << res->body;
                std::cout.flush();
            }

            return 0;
        }).detach();
    }

    int i = 0;
    std::string message;
    while (++i) {
        if (fLB::FLAGS_count_seconds) {
            message = std::to_string(i);
            std::this_thread::sleep_for(1s);
        } else {
            std::getline(std::cin, message);
        }
        auto res = cli->Post(fLS::FLAGS_resource.c_str(), headers, message, "text/plain");
        if (res == nullptr) {
            std::cerr << "No response\n";
            return 1;
        } else if (res->status != 200) {
            std::cerr << "Bad response " << res->status << '\n';
        }
    }
}
