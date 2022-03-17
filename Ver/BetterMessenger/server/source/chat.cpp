#include "chat.h"
#include "server.h"
#include "message.h"
#include "user.h"

using restbed::Session;

void Chat::addMessage(const Message &message) {
    std::unique_lock lock(mutex);
    for (auto &user : users) {
        user.second->yieldMessage(message);
    }
}

const std::unordered_map<std::string, std::shared_ptr<User>> &Chat::getUsers() const {
    return users;
}

std::shared_ptr<User> Chat::getUser(const std::string &name) const {
    std::shared_lock lock(mutex);
    if (users.count(name) == 0) return nullptr;
    else return users.at(name);
}

void Chat::addUser(const std::string &name, std::shared_ptr<Session> session) {
    std::unique_lock lock(mutex);
    users.insert({name, std::make_shared<User>(name, session)});
}
//}
