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

#include <iostream>

//using restbed::Session, restbed::Response;

//std::shared_ptr<Response> generate_response(const std::string &body, bool keep_alive = false)
//{
//    auto response = std::make_shared<Response>();
//    response->set_body(body);
//    response->set_header("Content-Length", std::to_string(body.size()));
//    if (keep_alive) {
//        response->set_header("Connection", "keep-alive");
//    } else {
//        response->set_header("Connection", "close");
//    }
//    response->set_status_code(200);
//    response->set_status_message("OK");
//    return response;
//}

namespace chat {
    struct Chat;

    struct Message {
    private:
        const std::string author;
        const std::string message;

    public:
        Message(std::string name, std::string text) : author(std::move(name)), message(std::move(text)) {}

        std::string generateMessage() {
            return author + ": " + message;
        }
    };

    struct User {
    private:
//        std::shared_ptr<Session> activeSession;
        std::string name;
        std::shared_ptr<Chat> chat;
        int index;
        std::queue<Message> messageQueue;
        std::shared_mutex mutex;
        std::condition_variable_any queueIsntEmpty;
//        std::condition_variable_any sessionIsActive;

//        void pushNewMessage(const Message &message) {
//            std::unique_lock lock(mutex);
//            messageQueue.push(message);
//            queueIsntEmpty.notify_all();
//        }

    public:
        User(std::string nm, std::shared_ptr<Chat> ch);
        void receiveMessage();
        void sendMessage(const std::string &text);

//        std::string yield() {
//            std::shared_lock lock(mutex);
//            queueIsntEmpty.wait(lock, [this]() {
//                return !messageQueue.empty();
//            });
//            std::string lastMessages;
//            while (!messageQueue.empty()) {
//                lastMessages += messageQueue.front().generateMessage();
//                lastMessages.push_back('\n');
//                messageQueue.pop();
//            }
//            return std::move(lastMessages);
//            std::cout << lastMessages;
//            auto response = generate_response(lastMessages, true);
//            while (activeSession == nullptr) {
//                sessionIsActive.wait(lock);
//            }
//            activeSession->yield(*response);
//        }
    };

    struct Chat {
    private:
        std::vector<Message> messages;
        std::shared_mutex mutex;
        std::condition_variable_any newMessage;

        void addMessage(const Message &message) {
            std::unique_lock lock(mutex);
            messages.push_back(message);
            newMessage.notify_all();
        }

        friend void User::sendMessage(const std::string &text);
        friend void User::receiveMessage();

    public:
        int messagesSize() {
            return messages.size();
        }
    };

    User::User(std::string nm, std::shared_ptr<Chat> ch) : name(std::move(nm)), chat(std::move(ch)), index(chat->messagesSize()) {}

    void User::receiveMessage() {
        std::shared_lock lock(chat->mutex);
        chat->newMessage.wait(lock, [this]() {
            return index < chat->messages.size();
        });
        while(index < chat->messages.size()) {
            std::cout << chat->messages[index++].generateMessage() << std::endl;
//            pushNewMessage(chat->messages[index++]);
        }
    }

    void User::sendMessage(const std::string &text) {
        chat->addMessage(Message{name, text});
    }
}


#endif //RESTAURANTBES_CHAT_H
