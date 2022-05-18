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

template<class T>
struct aggregate_adapter : public T {
    template<class... Args>
    aggregate_adapter(Args&&... args) : T{ std::forward<Args>(args)... } {}
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

void createOneColumnKeyboard(const std::vector<std::string> &buttonStrings,
                             TgBot::ReplyKeyboardMarkup::Ptr &kb) {
    for (const auto &buttonString : buttonStrings) {
        std::vector<TgBot::KeyboardButton::Ptr> row;
        TgBot::KeyboardButton::Ptr button(new TgBot::KeyboardButton);
        button->text = buttonString;
        row.push_back(button);
        kb->keyboard.push_back(row);
    }
}

void createKeyboard(const std::vector<std::vector<std::string>> &buttonLayout,
                    TgBot::ReplyKeyboardMarkup::Ptr &kb) {
    for (const auto &i : buttonLayout) {
        std::vector<TgBot::KeyboardButton::Ptr> row;
        for (const auto &j : i) {
            TgBot::KeyboardButton::Ptr button(new TgBot::KeyboardButton);
            button->text = j;
            row.push_back(button);
        }
        kb->keyboard.push_back(row);
    }
}

int main() {
    std::ifstream infile("/home/lizaerem/RestaurantBES/Liza/Telegram_Bot/TOKEN.txt");
    std::string TOKEN;
    infile >> TOKEN;
    TgBot::Bot bot(TOKEN);

    TgBot::ReplyKeyboardMarkup::Ptr keyboardDishStatus(
        new TgBot::ReplyKeyboardMarkup);
    createOneColumnKeyboard({"AVAILABLE", "UNAVAILABLE"}, keyboardDishStatus);

    TgBot::ReplyKeyboardMarkup::Ptr keyboardOrderStatus(
        new TgBot::ReplyKeyboardMarkup);
    createKeyboard({{"ACCEPTED", "IN_PROGRESS"}, {"CANCELLED"}, {"COMPLETED"}},
                   keyboardOrderStatus);

    std::vector<BotCommand::Ptr> commands;

    BotCommand::Ptr cmd(new BotCommand("start", "Log in"));
    commands.push_back(cmd);
    cmd = std::make_shared<aggregate_adapter<BotCommand>>("exit", "Log out");
    commands.push_back(cmd);
    cmd = std::make_shared<BotCommand>("set_order_status", "Change order status");
    commands.push_back(cmd);
    cmd = std::make_shared<BotCommand>("set_dish_status", "Change dish status");
    commands.push_back(cmd);
    cmd = std::make_shared<BotCommand>("set_dish_price", "Change dish price");
    commands.push_back(cmd);

    bot.getApi().setMyCommands(commands);
    std::vector<BotCommand::Ptr> vectCmd;

    TgBot::InlineKeyboardMarkup::Ptr keyboard(new TgBot::InlineKeyboardMarkup);
    std::vector<TgBot::InlineKeyboardButton::Ptr> row0;
    TgBot::InlineKeyboardButton::Ptr checkButton(
        new TgBot::InlineKeyboardButton);
    checkButton->text = "AVAILABLE";
    checkButton->callbackData = "AVAILABLE";
    row0.push_back(checkButton);
    checkButton = std::make_shared<TgBot::InlineKeyboardButton>();
    checkButton->text = "UNAVAILABLE";
    checkButton->callbackData = "UNAVAILABLE";
    row0.push_back(checkButton);
    keyboard->inlineKeyboard.push_back(row0);

    //    bot.getEvents().onCommand("start", [&bot, &keyboard](const
    //    Message::Ptr& message) {
    //        bot.getApi().sendMessage(message->chat->id, "Choose new status:",
    //        false, 0, keyboard);
    //    });
    //    bot.getEvents().onCallbackQuery([&bot, &keyboard](const
    //    TgBot::CallbackQuery::Ptr& query) {
    //        if (StringTools::startsWith(query->data, "AVAILABLE")) {
    //            std::string response = "ok";
    //            bot.getApi().sendMessage(query->message->chat->id, response,
    //            false, 0, keyboard, "Markdown");
    //        }
    //    });

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
                bot.getApi().sendMessage(message->chat->id, "Enter order id.");
                state[message->chat->id].waitOrderId = true;
            } else {
                bot.getApi().sendMessage(message->chat->id,
                                         "Access denied. Enter password.");
            }
        });

    bot.getEvents().onCommand(
        "set_dish_status", [&](const Message::Ptr &message) {
            if (state[message->chat->id].logged) {
                bot.getApi().sendMessage(
                    message->chat->id,
                    "Enter dish id. You entered " + message->text);

                state[message->chat->id].waitDishId = true;
                state[message->chat->id].toChangeDishStatus = true;
            } else {
                bot.getApi().sendMessage(message->chat->id,
                                         "Access denied. Enter password.");
            }
        });

    bot.getEvents().onCommand(
        "set_dish_price", [&](const Message::Ptr &message) {
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

    return 0;
}
