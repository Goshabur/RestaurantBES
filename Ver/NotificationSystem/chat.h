#ifndef RESTAURANTBES_CHAT_H
#define RESTAURANTBES_CHAT_H

#include <utility>
#include <vector>
#include <string>
#include <chrono>
#include <restbed>
#include <memory>
#include <queue>
#include <shared_mutex>
#include <condition_variable>
#include "server.h"

#include <iostream>

using restbed::Session, restbed::Response;

//namespace chat {
    struct Chat;

    struct Message {
    private:
        const std::string author;
        const std::string message;

    public:
        Message(std::string name, std::string text) : author(std::move(name)), message(std::move(text)) {}

        std::string generateMessage() { return author + ": " + message; }
    };

    struct User {
    private:
        std::string name;
        std::shared_ptr<Chat> chat;
        int index;
        std::shared_mutex mutex;

        std::shared_ptr<Session> activeSession;
        std::condition_variable_any sessionIsActive;

    public:
        User(std::string nm, std::shared_ptr<Chat> ch);
        std::string receiveMessages();
        void sendMessage(const std::string &text);
        void yieldMessages();
    };

    struct Chat {
    private:
        std::vector<Message> messages;
        std::shared_mutex mutex;
        std::condition_variable_any newMessage;

        void addMessage(const Message &message);

        friend void User::sendMessage(const std::string &text);
        friend std::string User::receiveMessages();

    public:
        int messagesSize() { return messages.size(); }
    };
//}


#endif //RESTAURANTBES_CHAT_H
