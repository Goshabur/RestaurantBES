#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <memory>
#include <string>
#include <tgbot/tgbot.h>
#include <unordered_map>
#include <vector>

using namespace TgBot;

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
  std::ifstream infile("/home/lizaerem/RestaurantBES/Telegram_Bot/TOKEN.txt");
  std::string TOKEN;
  infile >> TOKEN;

  Bot bot(TOKEN);

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
      bot.getApi().sendMessage(message->chat->id,
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
    if (!state[message->chat->id].logged && !check_admin(message->text)) {
      bot.getApi().sendMessage(message->chat->id, "Wrong password.");

    } else if (!state[message->chat->id].logged && check_admin(message->text)) {
      state[message->chat->id].logged = true;
      bot.getApi().sendMessage(message->chat->id, "Success.");

    } else {
      if (state[message->chat->id].waitOrderId) {
        if (!check_order(std::stoi(message->text))) {
          bot.getApi().sendMessage(message->chat->id, "Invalid id.");

        } else {
          state[message->chat->id].orderId = std::stoi(message->text);
          state[message->chat->id].waitOrderId = false;
          bot.getApi().sendMessage(message->chat->id,
                                   "Enter new status number.");
          state[message->chat->id].waitOrderStatus = true;
        }

      } else if (state[message->chat->id].waitOrderStatus) {
        state[message->chat->id].orderStatus = std::stoi(message->text);
        // change order status --> DB
        state[message->chat->id].waitOrderStatus = false;
        bot.getApi().sendMessage(message->chat->id,
                                 "Changed to " + message->text + ".");

      } else if (state[message->chat->id].waitDishId) {
        if (!check_dish_id(std::stoi(message->text))) {
          bot.getApi().sendMessage(message->chat->id, "Invalid id.");

        } else {
          state[message->chat->id].dishId = std::stoi(message->text);
          if (state[message->chat->id].toChangeDishPrice) {
            bot.getApi().sendMessage(message->chat->id, "Enter new price.");
            state[message->chat->id].waitDishPrice = true;
            state[message->chat->id].waitDishId = false;

          } else if (state[message->chat->id].toChangeDishStatus) {
            bot.getApi().sendMessage(message->chat->id,
                                     "Enter new status number.");
            state[message->chat->id].waitDishStatus = true;
            state[message->chat->id].waitDishId = false;
          }
        }

      } else if (state[message->chat->id].waitDishPrice) {
        state[message->chat->id].dishPrice = std::stoi(message->text);
        // change dish price --> DB
        state[message->chat->id].waitDishPrice = false;
        bot.getApi().sendMessage(message->chat->id,
                                 "Changed to " + message->text + ".");

      } else if (state[message->chat->id].waitDishStatus) {
        state[message->chat->id].dishStatus = std::stoi(message->text);
        // change dish status --> DB
        state[message->chat->id].waitDishStatus = false;
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
