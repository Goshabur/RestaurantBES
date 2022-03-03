#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"
#include <iostream>
#include <thread>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Invalid arguments\n";
    }
    std::string client_name = argv[1];
    auto cli = std::make_shared<httplib::Client>("https://localhost:1234");
    cli->enable_server_certificate_verification(false);
    cli->set_read_timeout(60);
    cli->set_keep_alive(true);

    std::thread([&client_name]() {
        auto cli2 = std::make_shared<httplib::Client>("https://localhost:1234");
        cli2->enable_server_certificate_verification(false);
        int i = 0;
        std::string message;
        while (++i) {
            message = std::to_string(i);
//            std::cin >> message;
            auto res = cli2->Post("/messenger", client_name + ": " + message, "text/plain");
            if (res == nullptr || res->status != 200) {
                std::cerr << "bad\n";
                return 1;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        }
        return 0;
    }).detach();

    while (true) {
        auto res = cli->Get("/messenger");
        if (res == nullptr) {
            std::cout << "No response\n";
            break;
        }
        if (res->status != 200) {
            std::cout << res->status << '\n';
            continue;
        }
        std::cout << res->body;
        std::cout.flush();
    }
}
