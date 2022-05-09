#include <tgbot/tgbot.h>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>

using namespace TgBot;

std::ifstream infile("TOKEN.txt");
const std::string TOKEN;
std::getline(infile, TOKEN);

class StateClass {
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

bool check_admin(const std::string &password) {
    // TODO: DB
    return "password" == password;
}

bool check_order(int id) {
    // TODO: DB
    return 20 == id;
}

bool check_dish_id(int id) {
    // TODO: DB
    return 15 == id;
}

int main() {
    Bot bot(TOKEN);

    std::vector <BotCommand::Ptr> commands;

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
    std::vector <BotCommand::Ptr> vectCmd;

    map_bool logged;
    map_bool waitOrderId;
    map_bool waitOrderStatus;
    map_int orderId;
    map_int orderStatus;
    map_bool waitDishId;
    map_bool waitDishStatus;
    map_bool waitDishPrice;
    map_int dishId;
    map_int dishStatus;
    map_int dishPrice;
    map_bool toChangeDishStatus;
    map_bool toChangeDishPrice;

    bot.getEvents().onCommand("start", [&](const Message::Ptr &message) {
        if (logged[message->chat->id]) {
            bot.getApi().sendMessage(message->chat->id, "Already logged in.");
        } else
            bot.getApi().sendMessage(
                    message->chat->id,
                    "Enter password to log in as an administrator.");
    });

    bot.getEvents().onCommand("order", [&](const Message::Ptr &message) {
        if (logged[message->chat->id]) {
            bot.getApi().sendMessage(message->chat->id, "Enter order id.");
            waitOrderId[message->chat->id] = true;
        } else {
            bot.getApi().sendMessage(message->chat->id,
                                     "Access denied. Enter password.");
        }
    });

    bot.getEvents().onCommand("dish_status", [&](const Message::Ptr &message) {
        if (logged[message->chat->id]) {
            bot.getApi().sendMessage(message->chat->id, "Enter dish id.");
            waitDishId[message->chat->id] = true;
            toChangeDishStatus[message->chat->id] = true;
        } else {
            bot.getApi().sendMessage(message->chat->id,
                                     "Access denied. Enter password.");
        }
    });

    bot.getEvents().onCommand("dish_price", [&](const Message::Ptr &message) {
        if (logged[message->chat->id]) {
            bot.getApi().sendMessage(message->chat->id, "Enter dish id.");
            waitDishId[message->chat->id] = true;
            toChangeDishPrice[message->chat->id] = true;
        } else {
            bot.getApi().sendMessage(message->chat->id,
                                     "Access denied. Enter password.");
        }
    });

    bot.getEvents().onUnknownCommand([&](const Message::Ptr &message) {
        bot.getApi().sendMessage(message->chat->id, "Unknown command.");
    });

    bot.getEvents().onNonCommandMessage([&](const Message::Ptr &message) {
        if (!logged[message->chat->id] && !check_admin(message->text)) {
            bot.getApi().sendMessage(message->chat->id, "Wrong password.");

        } else if (!logged[message->chat->id] && check_admin(message->text)) {
            logged[message->chat->id] = true;
            bot.getApi().sendMessage(message->chat->id, "Success.");

        } else {
            if (waitOrderId[message->chat->id]) {
                if (!check_order(std::stoi(message->text))) {
                    bot.getApi().sendMessage(message->chat->id, "Invalid id.");

                } else {
                    orderId[message->chat->id] = std::stoi(message->text);
                    waitOrderId[message->chat->id] = false;
                    bot.getApi().sendMessage(message->chat->id,
                                             "Enter new status number.");
                    waitOrderStatus[message->chat->id] = true;
                }

            } else if (waitOrderStatus[message->chat->id]) {
                orderStatus[message->chat->id] = std::stoi(message->text);
                // change order status --> DB
                waitOrderStatus[message->chat->id] = false;
                bot.getApi().sendMessage(message->chat->id,
                                         "Changed to " + message->text + ".");

            } else if (waitDishId[message->chat->id]) {
                if (!check_dish_id(std::stoi(message->text))) {
                    bot.getApi().sendMessage(message->chat->id, "Invalid id.");

                } else {
                    dishId[message->chat->id] = std::stoi(message->text);
                    if (toChangeDishPrice[message->chat->id]) {
                        bot.getApi().sendMessage(message->chat->id,
                                                 "Enter new price.");
                        waitDishPrice[message->chat->id] = true;
                        waitDishId[message->chat->id] = false;

                    } else if (toChangeDishStatus[message->chat->id]) {
                        bot.getApi().sendMessage(message->chat->id,
                                                 "Enter new status number.");
                        waitDishStatus[message->chat->id] = true;
                        waitDishId[message->chat->id] = false;
                    }
                }

            } else if (waitDishPrice[message->chat->id]) {
                dishPrice[message->chat->id] = std::stoi(message->text);
                // change dish price --> DB
                waitDishPrice[message->chat->id] = false;
                bot.getApi().sendMessage(message->chat->id,
                                         "Changed to " + message->text + ".");

            } else if (waitDishStatus[message->chat->id]) {
                dishStatus[message->chat->id] = std::stoi(message->text);
                // change dish status --> DB
                waitDishStatus[message->chat->id] = false;
                bot.getApi().sendMessage(message->chat->id,
                                         "Changed to " + message->text + ".");
            } else {
                bot.getApi().sendMessage(message->chat->id, "Unknown command.");
            }
        }
    });

    signal(SIGINT, [](int s) {
        printf("SIGINT got\n");
        exit(0);
    });

    try {
        bot.getApi().deleteWebhook();

        TgLongPoll longPoll(bot);
        while (true) {
            longPoll.start();
        }
    } catch (std::exception &e) {
        printf("error: %s\n", e.what());
    }

    return 0;
}
