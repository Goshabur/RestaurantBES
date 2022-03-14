#ifndef RESTAURANTBES_CHAT_H
#define RESTAURANTBES_CHAT_H

#include <restbed>
#include <string>
#include <shared_mutex>
#include <unordered_set>
#include <vector>
#include "fwd.h"

using restbed::Session, restbed::Response;

struct Chat {
private:
    std::vector<Message> messages;
    std::unordered_map<std::string, std::shared_ptr<User>> users;
    mutable std::shared_mutex mutex;

    friend User;

public:
    void addMessage(const Message &message);
    std::unordered_map<std::string, std::shared_ptr<User>> &getUsers();
    std::shared_ptr<User> getUser(const std::string &name) const;
    void addUser(const std::string &name, std::shared_ptr<Session> session);
};

#endif //RESTAURANTBES_CHAT_H
