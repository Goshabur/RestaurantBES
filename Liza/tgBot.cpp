#include "tgbot-cpp/include/tgbot/tgbot.h"
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "fwd.h"

using TgBot::BotCommand;
using TgBot::Message;

class StateClass {
public:
    bool logged;
    bool waitOrderId;
    bool waitOrderStatus;
    int orderId;
    int orderStatus;
    bool waitDishId;
    bool waitDishStatus;
    bool waitDishPrice;
    int dishId;
    int dishStatus;
    int dishPrice;
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
    std::ifstream infile("/home/lizaerem/RestaurantBES/Liza/TOKEN.txt");
    std::string TOKEN;
    infile >> TOKEN;

    TgBot::Bot bot(TOKEN);

    std::vector<BotCommand::Ptr> commands;

    BotCommand::Ptr cmdArray(new BotCommand);
    cmdArray->command = "start";
    cmdArray->description = "Log in";
    commands.push_back(cmdArray);

    cmdArray = std::make_shared<BotCommand>();
    cmdArray->command = "order";
    cmdArray->description = "Change order status";
    commands.push_back(cmdArray);

    cmdArray = std::make_shared<BotCommand>();
    cmdArray->command = "dish_status";
    cmdArray->description = "Change dish status";
    commands.push_back(cmdArray);

    cmdArray = std::make_shared<BotCommand>();
    cmdArray->command = "dish_price";
    cmdArray->description = "Set dish price";
    commands.push_back(cmdArray);

    bot.getApi().setMyCommands(commands);
    std::vector<BotCommand::Ptr> vectCmd;

    bot.getEvents().onCommand("start", [&](const Message::Ptr &message) {
        if (state[message->chat->id].logged) {
            bot.getApi().sendMessage(message->chat->id, "Already logged in.");
        } else
            bot.getApi().sendMessage(
                message->chat->id,
                "Enter password to log in as an administrator.");
    });

    bot.getEvents().onCommand("order", [&](const Message::Ptr &message) {
        if (state[message->chat->id].logged) {
            bot.getApi().sendMessage(message->chat->id, "Enter order id.");
            state[message->chat->id].waitOrderId = true;
        } else {
            bot.getApi().sendMessage(message->chat->id,
                                     "Access denied. Enter password.");
        }
    });

    bot.getEvents().onCommand("dish_status", [&](const Message::Ptr &message) {
        if (state[message->chat->id].logged) {
            bot.getApi().sendMessage(message->chat->id, "Enter dish id.");
            state[message->chat->id].waitDishId = true;
            state[message->chat->id].toChangeDishStatus = true;
        } else {
            bot.getApi().sendMessage(message->chat->id,
                                     "Access denied. Enter password.");
        }
    });

    bot.getEvents().onCommand("dish_price", [&](const Message::Ptr &message) {
        if (state[message->chat->id].logged) {
            bot.getApi().sendMessage(message->chat->id, "Enter dish id.");
            state[message->chat->id].waitDishId = true;
            state[message->chat->id].toChangeDishPrice = true;
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
                    state[message->chat->id].orderId = std::stoi(message->text);
                    state[message->chat->id].waitOrderId = false;
                    bot.getApi().sendMessage(
                        message->chat->id,
                        "Enter new status number (current status: " +
                            restbes::Admin::getOrderStatus(message->text) +
                            ").\n1: ACCEPTED\n2: "
                            "CANCELLED\n3: IN_PROGRESS\n4: COMPLETED");
                    state[message->chat->id].waitOrderStatus = true;
                }

            } else if (state[message->chat->id].waitOrderStatus) {
                if (!(message->text >= "1" && message->text <= "4")) {
                    bot.getApi().sendMessage(message->chat->id,
                                             "Invalid status!");
                } else {
                    state[message->chat->id].orderStatus =
                        std::stoi(message->text);
                    std::string oldStatus = restbes::Admin::getOrderStatus(
                        std::to_string(state[message->chat->id].orderId));
                    restbes::Admin::change_order_status(
                        std::to_string(state[message->chat->id].orderId),
                        std::stoi(message->text));
                    bot.getApi().sendMessage(
                        message->chat->id,
                        "Changed from \"" + oldStatus + "\" to \"" +
                            restbes::orderStatuses[std::stoi(message->text)] +
                            "\".");
                    state[message->chat->id].waitOrderStatus = false;
                }

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
                                " (id: " + message->text +
                                ", current price: " + restbes::Admin::getPrice(message->text) + ").");
                        state[message->chat->id].waitDishPrice = true;
                        state[message->chat->id].waitDishId = false;

                    } else if (state[message->chat->id].toChangeDishStatus) {
                        bot.getApi().sendMessage(
                            message->chat->id,
                            "Enter new status number for " +
                                restbes::Admin::getDishName(message->text) +
                                " (id: " + message->text +
                                ", current status: " + restbes::Admin::getDishStatus(message->text) + ").\n0: "
                                "UNAVAILABLE\n1: AVAILABLE");
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
                if (!(message->text == "1" || message->text == "0")) {
                    bot.getApi().sendMessage(message->chat->id,
                                             "Invalid status!");

                } else {
                    state[message->chat->id].dishStatus =
                        std::stoi(message->text);
                    std::string oldStatus = restbes::Admin::getDishStatus(
                        std::to_string(state[message->chat->id].dishId));
                    if (oldStatus == restbes::dishStatuses[std::stoi(message->text)]) {
                        bot.getApi().sendMessage(message->chat->id,
                                                 "Nothing to change.");
                    } else {
                        restbes::Admin::change_dish_status(
                            std::to_string(state[message->chat->id].dishId),
                            std::stoi(message->text));
                        bot.getApi().sendMessage(
                            message->chat->id,
                            "Changed from \"" + oldStatus + "\" to \"" +
                                restbes::dishStatuses[std::stoi(
                                    message->text)] +
                                "\".");
                    }
                    state[message->chat->id].waitDishStatus = false;
                }

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

    return 0;
}
