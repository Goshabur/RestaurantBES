#include "message.h"

Message::Message(std::string name, std::string text) : author(std::move(name)), message(std::move(text)) {}

[[nodiscard]] std::string Message::generateMessage() const {
    return author + ": " + message + '\n';
}