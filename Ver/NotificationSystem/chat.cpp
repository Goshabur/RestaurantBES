#include "chat.h"

//namespace chat {
    User::User(std::string nm, std::shared_ptr<Chat> ch) : name(std::move(nm)), chat(std::move(ch)), index(chat->messagesSize()) {}

    std::string User::receiveMessages() {
        std::shared_lock lock(chat->mutex);
        chat->newMessage.wait(lock, [this]() {
            return index < chat->messages.size();
        });
        std::string lastMessages;
        while(index < chat->messages.size()) {
            lastMessages += chat->messages[index++].generateMessage();
            lastMessages.push_back('\n');
        }
        return std::move(lastMessages);
    }

    void User::yieldMessages() {
        std::string responseBody = receiveMessages();
        auto response = generate_response(responseBody, true);
        std::shared_lock lock(mutex);
        while (activeSession == nullptr) {
            sessionIsActive.wait(lock);
        }
        activeSession->yield(*response);
    }

    void User::sendMessage(const std::string &text) {
        chat->addMessage(Message{name, text});
    }

    void Chat::addMessage(const Message &message) {
        std::unique_lock lock(mutex);
        messages.push_back(message);
        newMessage.notify_all();
    }
//}
