#ifndef RESTAURANTBES_CHAT_H
#define RESTAURANTBES_CHAT_H

#include <restbed>
#include <string>
#include <shared_mutex>
#include <unordered_set>
#include <vector>
#include "fwd.h"
#include "user.h"

using restbed::Session;

struct Chat {
private:
    std::unordered_map<std::string, std::shared_ptr<User>> users;
    mutable std::shared_mutex mutex;

    void addMessage(const Message &message);

    friend void User::sendMessage(const std::string &text, std::shared_ptr<Chat> chat) const;

public:
    [[nodiscard]] const std::unordered_map<std::string, std::shared_ptr<User>> &getUsers() const;
    [[nodiscard]] std::shared_ptr<User> getUser(const std::string &name) const;
    void addUser(const std::string &name, std::shared_ptr<Session> session);
};

#endif //RESTAURANTBES_CHAT_H
