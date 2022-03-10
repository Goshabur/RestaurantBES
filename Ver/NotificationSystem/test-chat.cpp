#include "chat.h"
#include <thread>

using namespace chat;
using namespace std::literals;

int main() {
    auto chat = std::make_shared<Chat>();
    std::vector<std::shared_ptr<User>> vec = {std::make_shared<User>("Alice", chat), std::make_shared<User>("Bob", chat), std::make_shared<User>("Charlie", chat)};

    for (auto &usr : vec) {
        std::thread([&usr]() {
            int i = 0;
            while(++i) {
                usr->sendMessage("hello " + std::to_string(i));
                std::this_thread::sleep_for(1s);
            }
        }).detach();
        std::this_thread::sleep_for(300ms);
    }

    while(true) {
        vec[0]->receiveMessage();
    }
}