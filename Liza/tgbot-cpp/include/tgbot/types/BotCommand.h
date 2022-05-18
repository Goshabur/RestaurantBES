#ifndef TGBOT_BOTCOMMAND_H
#define TGBOT_BOTCOMMAND_H

#include <cstdint>
#include <memory>
#include <string>
#include <utility>

namespace TgBot {

/**
 * @brief This object represents a bot command.
 *
 * https://core.telegram.org/bots/api#botcommand
 * @ingroup types
 */
class BotCommand {
public:
    typedef std::shared_ptr<BotCommand> Ptr;
    BotCommand() {
    }

    explicit BotCommand(std::string command_, std::string description_)
        : command(std::move(command_)), description(std::move(description_)) {
    }

    virtual ~BotCommand() {
    }

    /**
     * @brief command label.
     */
    std::string command;

    /**
     * @brief description label.
     */
    std::string description;
};
}  // namespace TgBot

#endif  // TGBOT_BOTCOMMAND_H
