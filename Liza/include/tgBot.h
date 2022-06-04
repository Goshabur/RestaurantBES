#pragma once

#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "admin.h"
#include "fwd.h"
#include "tgbot/tgbot.h"

using TgBot::BotCommand;
using TgBot::Message;

template <class T>
struct aggregate_adapter : public T {
    template <class... Args>
    explicit aggregate_adapter(Args &&...args)
        : T{std::forward<Args>(args)...} {
    }
};

namespace TelegramBot {
class StateClass {
public:
    bool logged;
    bool waitOrderId;
    bool waitOrderStatus;
    bool waitNewDishName;
    bool waitNewDishPrice;
    bool waitNewDishInfo;
    bool waitNewDishImage;
    int orderId;
    bool waitDishId;
    bool waitDishStatus;
    bool waitDishPrice;
    int dishId;
    bool toChangeDishStatus;
    bool toChangeDishPrice;
    std::string newDishName;
    std::string newDishPrice;
    std::string newDishInfo;
    std::string newDishImage;
};

static inline std::unordered_map<int64_t, StateClass> state;

void tgBotPolling();

void clear_state(int chat_id);

bool check_number(const std::string &num);

}  // namespace TelegramBot
