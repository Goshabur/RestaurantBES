#include "tgBot.h"

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

void TelegramBot::tgBotPolling() {
    std::ifstream infile("/home/lizaerem/RestaurantBES/Liza/Secure/TOKEN.txt");
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
    cmd = std::make_shared<BotCommand>("add_dish", "Add new dish to menu");
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
        clear_state(message->chat->id);
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

    bot.getEvents().onCommand("set_order_status", [&](const Message::Ptr
                                                          &message) {
        clear_state(message->chat->id);
        if (state[message->chat->id].logged) {
            std::stringstream ss(message->text);
            std::string command, number;
            ss >> command;
            ss >> number;
            if (number.empty()) {
                bot.getApi().sendMessage(message->chat->id, "Enter order id.");
                state[message->chat->id].waitOrderId = true;
            } else {
                if (!restbesAdmin::check_order_exists(number)) {
                    bot.getApi().sendMessage(message->chat->id,
                                             "No order with such id.");
                    state[message->chat->id].waitOrderId = true;
                } else {
                    bot.getApi().sendMessage(
                        message->chat->id,
                        "Choose new status (current status: \"" +
                            restbes::orderStatuses[restbesAdmin::getOrderStatus(
                                number)] +
                            "\").",
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
                int oldStatus = restbesAdmin::getOrderStatus(
                    std::to_string(state[query->message->chat->id].orderId));
                if (restbes::orderStatuses[oldStatus] == query->data) {
                    bot.getApi().sendMessage(query->message->chat->id,
                                             "Nothing to change.");
                } else {
                    restbesAdmin::change_order_status(
                        std::to_string(state[query->message->chat->id].orderId),
                        std::to_string(restbes::orderStatusesMap[query->data]));
                    bot.getApi().sendMessage(
                        query->message->chat->id,
                        "Changed from \"" + restbes::orderStatuses[oldStatus] +
                            "\" to \"" + orderStatus + "\".");
                }
                state[query->message->chat->id].waitOrderStatus = false;
            } else if (state[query->message->chat->id].waitDishStatus) {
                std::string dishStatus = query->data;
                int oldStatus = restbesAdmin::getDishStatus(
                    std::to_string(state[query->message->chat->id].dishId));
                if (restbes::dishStatuses[oldStatus] == query->data) {
                    bot.getApi().sendMessage(query->message->chat->id,
                                             "Nothing to change.");
                } else {
                    restbesAdmin::change_dish_status(
                        std::to_string(state[query->message->chat->id].dishId),
                        std::to_string(restbes::dishStatusesMap[query->data]));
                    bot.getApi().sendMessage(
                        query->message->chat->id,
                        "Changed from \"" + restbes::dishStatuses[oldStatus] +
                            "\" to \"" + dishStatus + "\".");
                }
                state[query->message->chat->id].waitDishStatus = false;
            }
        });

    bot.getEvents().onCommand("set_dish_status", [&](const Message::Ptr
                                                         &message) {
        clear_state(message->chat->id);
        if (state[message->chat->id].logged) {
            std::stringstream ss(message->text);
            std::string command, number;
            ss >> command;
            ss >> number;
            if (number.empty()) {
                bot.getApi().sendMessage(message->chat->id, "Enter dish id.");
                state[message->chat->id].waitDishId = true;
                state[message->chat->id].toChangeDishStatus = true;
            } else {
                if (!restbesAdmin::check_dish_exists(number)) {
                    bot.getApi().sendMessage(message->chat->id,
                                             "No dish with such id.");
                    state[message->chat->id].waitDishId = true;
                } else {
                    state[message->chat->id].dishId = std::stoi(number);
                    bot.getApi().sendMessage(
                        message->chat->id,
                        "Choose new status for " +
                            restbesAdmin::getDishName(number) +
                            " (id: " + number + ", current status: \"" +
                            restbes::dishStatuses[restbesAdmin::getDishStatus(
                                number)] +
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
            clear_state(message->chat->id);
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
                    if (!restbesAdmin::check_dish_exists(number)) {
                        bot.getApi().sendMessage(message->chat->id,
                                                 "No dish with such id.");
                        state[message->chat->id].waitDishId = true;
                    } else {
                        state[message->chat->id].dishId = std::stoi(number);
                        bot.getApi().sendMessage(
                            message->chat->id,
                            "Enter new price for " +
                                restbesAdmin::getDishName(number) +
                                " (id: " + number + ", current price: " +
                                restbesAdmin::getPrice(number) + ").");
                        state[message->chat->id].waitDishPrice = true;
                        state[message->chat->id].waitDishId = false;
                    }
                }
            } else {
                bot.getApi().sendMessage(message->chat->id,
                                         "Access denied. Enter password.");
            }
        });

    bot.getEvents().onCommand("add_dish", [&](const Message::Ptr &message) {
        clear_state(message->chat->id);
        if (state[message->chat->id].logged) {
            bot.getApi().sendMessage(message->chat->id, "Enter dish name.");
            state[message->chat->id].waitNewDishName = true;

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
            !restbesAdmin::check_admin(message->text)) {
            bot.getApi().sendMessage(message->chat->id, "Wrong password.");

        } else if (!state[message->chat->id].logged &&
                   restbesAdmin::check_admin(message->text)) {
            state[message->chat->id].logged = true;
            bot.getApi().sendMessage(message->chat->id, "Success.");

        } else {
            if (state[message->chat->id].waitOrderId) {
                if (!restbesAdmin::check_order_exists(message->text)) {
                    bot.getApi().sendMessage(message->chat->id,
                                             "No order with such id.");

                } else {
                    bot.getApi().sendMessage(
                        message->chat->id,
                        "Choose new status (current status: \"" +
                            restbes::orderStatuses[restbesAdmin::getOrderStatus(
                                message->text)] +
                            "\").",
                        false, 0, keyboardOrderStatus);
                    state[message->chat->id].orderId = std::stoi(message->text);
                    state[message->chat->id].waitOrderId = false;
                    state[message->chat->id].waitOrderStatus = true;
                }

            } else if (state[message->chat->id].waitOrderStatus) {
                std::string orderStatus = message->text;
                int oldStatus = restbesAdmin::getOrderStatus(
                    std::to_string(state[message->chat->id].orderId));
                if (restbes::orderStatuses[oldStatus] == message->text) {
                    bot.getApi().sendMessage(message->chat->id,
                                             "Nothing to change.");
                } else {
                    restbesAdmin::change_order_status(
                        std::to_string(state[message->chat->id].orderId),
                        std::to_string(
                            restbes::orderStatusesMap[message->text]));
                    bot.getApi().sendMessage(
                        message->chat->id,
                        "Changed from \"" + restbes::orderStatuses[oldStatus] +
                            "\" to \"" + orderStatus + "\".");
                }
                state[message->chat->id].waitOrderStatus = false;

            } else if (state[message->chat->id].waitDishId) {
                if (!restbesAdmin::check_dish_exists(message->text)) {
                    bot.getApi().sendMessage(message->chat->id,
                                             "No dish with such id.");

                } else {
                    state[message->chat->id].dishId = std::stoi(message->text);
                    if (state[message->chat->id].toChangeDishPrice) {
                        bot.getApi().sendMessage(
                            message->chat->id,
                            "Enter new price for " +
                                restbesAdmin::getDishName(message->text) +
                                " (id: " + message->text + ", current price: " +
                                restbesAdmin::getPrice(message->text) + ").");
                        state[message->chat->id].waitDishPrice = true;
                        state[message->chat->id].waitDishId = false;

                    } else if (state[message->chat->id].toChangeDishStatus) {
                        bot.getApi().sendMessage(
                            message->chat->id,
                            "Choose new status for " +
                                restbesAdmin::getDishName(message->text) +
                                " (id: " + message->text +
                                ", current status: \"" +
                                restbes::dishStatuses
                                    [restbesAdmin::getDishStatus(
                                        message->text)] +
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
                    std::string oldPrice = restbesAdmin::getPrice(
                        std::to_string(state[message->chat->id].dishId));
                    if (oldPrice == message->text) {
                        bot.getApi().sendMessage(message->chat->id,
                                                 "Nothing to change.");
                    } else {
                        restbesAdmin::change_dish_price(
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
                int oldStatus = restbesAdmin::getDishStatus(
                    std::to_string(state[message->chat->id].dishId));
                if (restbes::dishStatuses[oldStatus] == dishStatus) {
                    bot.getApi().sendMessage(message->chat->id,
                                             "Nothing to change.");
                } else {
                    restbesAdmin::change_dish_status(
                        std::to_string(state[message->chat->id].dishId),
                        dishStatus);
                    bot.getApi().sendMessage(
                        message->chat->id,
                        "Changed from \"" + restbes::dishStatuses[oldStatus] +
                            "\" to \"" + dishStatus + "\".");
                }
                state[message->chat->id].waitDishStatus = false;

            } else if (state[message->chat->id].waitNewDishName) {
                state[message->chat->id].newDishName = message->text;
                state[message->chat->id].waitNewDishName = false;
                bot.getApi().sendMessage(message->chat->id,
                                         "Enter dish price.");
                state[message->chat->id].waitNewDishPrice = true;

            } else if (state[message->chat->id].waitNewDishPrice) {
                if (!check_number(message->text)) {
                    bot.getApi().sendMessage(message->chat->id,
                                             "Invalid price!");
                } else {
                    state[message->chat->id].newDishPrice = message->text;
                    state[message->chat->id].waitNewDishPrice = false;
                    bot.getApi().sendMessage(message->chat->id,
                                             "Enter dish info (description).");
                    state[message->chat->id].waitNewDishInfo = true;
                }

            } else if (state[message->chat->id].waitNewDishInfo) {
                state[message->chat->id].newDishInfo = message->text;
                state[message->chat->id].waitNewDishInfo = false;
                bot.getApi().sendMessage(message->chat->id,
                                         "Enter dish image URL.");
                state[message->chat->id].waitNewDishImage = true;

            } else if (state[message->chat->id].waitNewDishImage) {
                // TODO (optional): check for valid url;
                state[message->chat->id].newDishImage = message->text;
                state[message->chat->id].waitNewDishImage = false;

                std::string id = restbesAdmin::add_new_dish(
                    state[message->chat->id].newDishName,
                    state[message->chat->id].newDishPrice,
                    state[message->chat->id].newDishImage);

                state[message->chat->id].newDishName.clear();
                state[message->chat->id].newDishPrice.clear();
                state[message->chat->id].newDishInfo.clear();
                state[message->chat->id].newDishImage.clear();

                bot.getApi().sendMessage(
                    message->chat->id,
                    "Successfully added. Dish' id — " + id + ".");

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
