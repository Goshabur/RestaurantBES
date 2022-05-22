#include "tgbot/tgbot.h"
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "../include/fwd.h"

using TgBot::BotCommand;
using TgBot::Message;

template <class T>
struct aggregate_adapter : public T {
    template <class... Args>
    explicit aggregate_adapter(Args &&...args)
        : T{std::forward<Args>(args)...} {
    }
};

class StateClass {
public:
    bool logged;
    bool waitOrderId;
    bool waitOrderStatus;
    int orderId;
    bool waitDishId;
    bool waitDishStatus;
    bool waitDishPrice;
    int dishId;
    bool toChangeDishStatus;
    bool toChangeDishPrice;
};
std::unordered_map<int64_t, StateClass> state;

bool check_number(const std::string &num) {
    if (num[0] == '0') {
        return false;
    }

    return std::all_of(num.begin(), num.end(),
                       [](char x) { return isdigit(x); });
}

int main() {
    std::ifstream infile(
        "/home/lizaerem/RestaurantBES/Liza/Telegram_Bot/TOKEN.txt");
    std::string TOKEN;
    infile >> TOKEN;
    TgBot::Bot bot(TOKEN);

    std::vector<BotCommand::Ptr> commands;

    BotCommand::Ptr cmd(new BotCommand("start", "Log in"));
    commands.push_back(cmd);
    cmd = std::make_shared<aggregate_adapter<BotCommand>>("exit", "Log out");
    commands.push_back(cmd);
    cmd =
        std::make_shared<BotCommand>("set_order_status", "Change order status");
    commands.push_back(cmd);
    cmd = std::make_shared<BotCommand>("set_dish_status", "Change dish status");
    commands.push_back(cmd);
    cmd = std::make_shared<BotCommand>("set_dish_price", "Change dish price");
    commands.push_back(cmd);

    bot.getApi().setMyCommands(commands);
    std::vector<BotCommand::Ptr> vectCmd;

    TgBot::InlineKeyboardMarkup::Ptr keyboardDishStatus(
        new TgBot::InlineKeyboardMarkup);
    std::vector<TgBot::InlineKeyboardButton::Ptr> row00;
    TgBot::InlineKeyboardButton::Ptr dishStatusButton(
        new TgBot::InlineKeyboardButton);
    dishStatusButton->text = "AVAILABLE";
    dishStatusButton->callbackData = "AVAILABLE";
    row00.push_back(dishStatusButton);
    dishStatusButton = std::make_shared<TgBot::InlineKeyboardButton>();
    dishStatusButton->text = "UNAVAILABLE";
    dishStatusButton->callbackData = "UNAVAILABLE";
    row00.push_back(dishStatusButton);
    keyboardDishStatus->inlineKeyboard.push_back(row00);

    TgBot::InlineKeyboardMarkup::Ptr keyboardOrderStatus(
        new TgBot::InlineKeyboardMarkup);
    std::vector<TgBot::InlineKeyboardButton::Ptr> row01;
    TgBot::InlineKeyboardButton::Ptr orderStatusButton(
        new TgBot::InlineKeyboardButton);
    orderStatusButton->text = "ACCEPTED";
    orderStatusButton->callbackData = "ACCEPTED";
    row01.push_back(orderStatusButton);
    orderStatusButton = std::make_shared<TgBot::InlineKeyboardButton>();
    orderStatusButton->text = "IN_PROGRESS";
    orderStatusButton->callbackData = "IN_PROGRESS";
    row01.push_back(orderStatusButton);
    orderStatusButton = std::make_shared<TgBot::InlineKeyboardButton>();
    orderStatusButton->text = "CANCELLED";
    orderStatusButton->callbackData = "CANCELLED";
    row01.push_back(orderStatusButton);
    orderStatusButton = std::make_shared<TgBot::InlineKeyboardButton>();
    orderStatusButton->text = "COMPLETED";
    orderStatusButton->callbackData = "COMPLETED";
    row01.push_back(orderStatusButton);
    keyboardOrderStatus->inlineKeyboard.push_back(row01);

    bot.getEvents().onCommand("start", [&](const Message::Ptr &message) {
        if (state[message->chat->id].logged) {
            bot.getApi().sendMessage(message->chat->id, "Already logged in.");
        } else
            bot.getApi().sendMessage(
                message->chat->id,
                "Enter password to log in as an administrator.");
    });

    bot.getEvents().onCommand("exit", [&](const Message::Ptr &message) {
        state.erase(message->chat->id);
        bot.getApi().sendMessage(message->chat->id, "Signed out.");
    });

    bot.getEvents().onCommand(
        "set_order_status", [&](const Message::Ptr &message) {
            if (state[message->chat->id].logged) {
                std::stringstream ss(message->text);
                std::string command, number;
                ss >> command;
                ss >> number;
                if (number.empty()) {
                    bot.getApi().sendMessage(message->chat->id,
                                             "Enter order id.");
                    state[message->chat->id].waitOrderId = true;
                } else {
                    if (!restbes::check_order_exists(number)) {
                        bot.getApi().sendMessage(message->chat->id,
                                                 "No order with such id.");
                        state[message->chat->id].waitOrderId = true;
                    } else {
                        bot.getApi().sendMessage(
                            message->chat->id,
                            "Choose new status (current status: \"" +
                                restbes::Admin::getOrderStatus(number) + "\").",
                            false, 0, keyboardOrderStatus);
                        state[message->chat->id].orderId = std::stoi(number);
                        state[message->chat->id].waitOrderId = false;
                        state[message->chat->id].waitOrderStatus = true;
                    }
                }
            } else {
                bot.getApi().sendMessage(message->chat->id,
                                         "Access denied. Enter password.");
            }
        });

    bot.getEvents().onCallbackQuery(
        [&bot](const TgBot::CallbackQuery::Ptr &query) {
            if (state[query->message->chat->id].waitOrderStatus) {
                std::string orderStatus = query->data;
                std::string oldStatus = restbes::Admin::getOrderStatus(
                    std::to_string(state[query->message->chat->id].orderId));
                if (oldStatus == query->data) {
                    bot.getApi().sendMessage(query->message->chat->id,
                                             "Nothing to change.");
                } else {
                    restbes::Admin::change_order_status(
                        std::to_string(state[query->message->chat->id].orderId),
                        query->data);
                    bot.getApi().sendMessage(query->message->chat->id,
                                             "Changed from \"" + oldStatus +
                                                 "\" to \"" + orderStatus +
                                                 "\".");
                }
                state[query->message->chat->id].waitOrderStatus = false;
            } else if (state[query->message->chat->id].waitDishStatus) {
                std::string dishStatus = query->data;
                std::string oldStatus = restbes::Admin::getDishStatus(
                    std::to_string(state[query->message->chat->id].dishId));
                if (oldStatus == query->data) {
                    bot.getApi().sendMessage(query->message->chat->id,
                                             "Nothing to change.");
                } else {
                    restbes::Admin::change_dish_status(
                        std::to_string(state[query->message->chat->id].dishId),
                        query->data);
                    bot.getApi().sendMessage(query->message->chat->id,
                                             "Changed from \"" + oldStatus +
                                                 "\" to \"" + dishStatus +
                                                 "\".");
                }
                state[query->message->chat->id].waitDishStatus = false;
            }
        });

    bot.getEvents().onCommand(
        "set_dish_status", [&](const Message::Ptr &message) {
            if (state[message->chat->id].logged) {
                std::stringstream ss(message->text);
                std::string command, number;
                ss >> command;
                ss >> number;
                if (number.empty()) {
                    bot.getApi().sendMessage(message->chat->id,
                                             "Enter dish id.");
                    state[message->chat->id].waitDishId = true;
                    state[message->chat->id].toChangeDishStatus = true;
                } else {
                    if (!restbes::check_dish_exists(number)) {
                        bot.getApi().sendMessage(message->chat->id,
                                                 "No dish with such id.");
                        state[message->chat->id].waitDishId = true;
                    } else {
                        state[message->chat->id].dishId = std::stoi(number);
                        bot.getApi().sendMessage(
                            message->chat->id,
                            "Choose new status for " +
                                restbes::Admin::getDishName(number) +
                                " (id: " + number + ", current status: \"" +
                                restbes::Admin::getDishStatus(number) +
                                "\").\n",
                            false, 0, keyboardDishStatus);
                        state[message->chat->id].waitDishStatus = true;
                        state[message->chat->id].waitDishId = false;
                    }
                }
            } else {
                bot.getApi().sendMessage(message->chat->id,
                                         "Access denied. Enter password.");
            }
        });

    bot.getEvents().onCommand(
        "set_dish_price", [&](const Message::Ptr &message) {
            if (state[message->chat->id].logged) {
                std::stringstream ss(message->text);
                std::string command, number;
                ss >> command;
                ss >> number;
                if (number.empty()) {
                    bot.getApi().sendMessage(message->chat->id,
                                             "Enter dish id.");
                    state[message->chat->id].waitDishId = true;
                    state[message->chat->id].toChangeDishPrice = true;
                } else {
                    if (!restbes::check_dish_exists(number)) {
                        bot.getApi().sendMessage(message->chat->id,
                                                 "No dish with such id.");
                        state[message->chat->id].waitDishId = true;
                    } else {
                        state[message->chat->id].dishId = std::stoi(number);
                        bot.getApi().sendMessage(
                            message->chat->id,
                            "Enter new price for " +
                                restbes::Admin::getDishName(number) +
                                " (id: " + number + ", current price: " +
                                restbes::Admin::getPrice(number) + ").");
                        state[message->chat->id].waitDishPrice = true;
                        state[message->chat->id].waitDishId = false;
                    }
                }
            } else {
                bot.getApi().sendMessage(message->chat->id,
                                         "Access denied. Enter password.");
            }
        });

    bot.getEvents().onUnknownCommand([&](const Message::Ptr &message) {
        bot.getApi().sendMessage(message->chat->id, "Unknown command.");
    });

    bot.getEvents().onNonCommandMessage([&](const Message::Ptr &message) {
        if (!state[message->chat->id].logged &&
            !restbes::check_admin(message->text)) {
            bot.getApi().sendMessage(message->chat->id, "Wrong password.");

        } else if (!state[message->chat->id].logged &&
                   restbes::check_admin(message->text)) {
            state[message->chat->id].logged = true;
            bot.getApi().sendMessage(message->chat->id, "Success.");

        } else {
            if (state[message->chat->id].waitOrderId) {
                if (!restbes::check_order_exists(message->text)) {
                    bot.getApi().sendMessage(message->chat->id,
                                             "No order with such id.");

                } else {
                    bot.getApi().sendMessage(
                        message->chat->id,
                        "Choose new status (current status: \"" +
                            restbes::Admin::getOrderStatus(message->text) +
                            "\").",
                        false, 0, keyboardOrderStatus);
                    state[message->chat->id].orderId = std::stoi(message->text);
                    state[message->chat->id].waitOrderId = false;
                    state[message->chat->id].waitOrderStatus = true;
                }

            } else if (state[message->chat->id].waitOrderStatus) {
                std::string orderStatus = message->text;
                std::string oldStatus = restbes::Admin::getOrderStatus(
                    std::to_string(state[message->chat->id].orderId));
                if (oldStatus == message->text) {
                    bot.getApi().sendMessage(message->chat->id,
                                             "Nothing to change.");
                } else {
                    restbes::Admin::change_order_status(
                        std::to_string(state[message->chat->id].orderId),
                        message->text);
                    bot.getApi().sendMessage(message->chat->id,
                                             "Changed from \"" + oldStatus +
                                                 "\" to \"" + orderStatus +
                                                 "\".");
                }
                state[message->chat->id].waitOrderStatus = false;

            } else if (state[message->chat->id].waitDishId) {
                if (!restbes::check_dish_exists(message->text)) {
                    bot.getApi().sendMessage(message->chat->id,
                                             "No dish with such id.");

                } else {
                    state[message->chat->id].dishId = std::stoi(message->text);
                    if (state[message->chat->id].toChangeDishPrice) {
                        bot.getApi().sendMessage(
                            message->chat->id,
                            "Enter new price for " +
                                restbes::Admin::getDishName(message->text) +
                                " (id: " + message->text + ", current price: " +
                                restbes::Admin::getPrice(message->text) + ").");
                        state[message->chat->id].waitDishPrice = true;
                        state[message->chat->id].waitDishId = false;

                    } else if (state[message->chat->id].toChangeDishStatus) {
                        bot.getApi().sendMessage(
                            message->chat->id,
                            "Choose new status for " +
                                restbes::Admin::getDishName(message->text) +
                                " (id: " + message->text +
                                ", current status: \"" +
                                restbes::Admin::getDishStatus(message->text) +
                                "\").\n",
                            false, 0, keyboardDishStatus);
                        state[message->chat->id].waitDishStatus = true;
                        state[message->chat->id].waitDishId = false;
                    }
                }

            } else if (state[message->chat->id].waitDishPrice) {
                if (!check_number(message->text)) {
                    bot.getApi().sendMessage(message->chat->id,
                                             "Invalid price!");
                } else {
                    std::string oldPrice = restbes::Admin::getPrice(
                        std::to_string(state[message->chat->id].dishId));
                    if (oldPrice == message->text) {
                        bot.getApi().sendMessage(message->chat->id,
                                                 "Nothing to change.");
                    } else {
                        restbes::Admin::change_dish_price(
                            std::to_string(state[message->chat->id].dishId),
                            message->text);
                        bot.getApi().sendMessage(message->chat->id,
                                                 "Changed from " + oldPrice +
                                                     " to " + message->text +
                                                     ".");
                    }
                    state[message->chat->id].waitDishPrice = false;
                }

            } else if (state[message->chat->id].waitDishStatus) {
                std::string dishStatus = message->text;
                std::string oldStatus = restbes::Admin::getDishStatus(
                    std::to_string(state[message->chat->id].dishId));
                if (oldStatus == dishStatus) {
                    bot.getApi().sendMessage(message->chat->id,
                                             "Nothing to change.");
                } else {
                    restbes::Admin::change_dish_status(
                        std::to_string(state[message->chat->id].dishId),
                        dishStatus);
                    bot.getApi().sendMessage(
                        message->chat->id, "Changed from \"" + oldStatus +
                                               "\" to \"" + dishStatus + "\".");
                }
                state[message->chat->id].waitDishStatus = false;

            } else {
                bot.getApi().sendMessage(message->chat->id, "Unknown command.");
            }
        }
    });

    signal(SIGINT, [](int s) {
        printf("SIGINT got\n");
        exit(0);
    });

    bot.getApi().deleteWebhook();

    TgBot::TgLongPoll longPoll(bot);
    while (true) {
        try {
            longPoll.start();
        } catch (std::exception &e) {
            printf("error: %s\n", e.what());
        }
    }
}
