#include "tgBot.h"
//#include <thread>

namespace TelegramBot {

void clear_state(int chat_id) {
    bool logged = state[chat_id].logged;
    state[chat_id] = StateClass();
    state[chat_id].logged = logged;
}

bool check_number(const std::string &num) {
    if (num[0] == '0') {
        return false;
    }

    return std::all_of(num.begin(), num.end(),
                       [](char x) { return isdigit(x); });
}
}  // namespace TelegramBot

int main() {
    std::thread t([] {
        TelegramBot::tgBotPolling();
    });
    t.detach();
}
