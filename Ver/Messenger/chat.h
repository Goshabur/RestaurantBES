#ifndef RESTAURANTBES_CHAT_H
#define RESTAURANTBES_CHAT_H

#include <vector>
#include <string>
#include <condition_variable>
#include <shared_mutex>

struct Chat {
private:
    static inline std::vector<std::string> messages;
    std::size_t iterator;
    static inline std::shared_mutex mutex;
    static inline std::condition_variable_any cond_var;

public:
    Chat() : iterator(messages.size()) {}

    static void send(const std::string &message) {
        std::unique_lock lock(mutex);
        messages.push_back(message);
        cond_var.notify_all();
    }

    std::string receive() {
        std::string last_messages;
        std::shared_lock lock(mutex);
        cond_var.wait(lock, [this]() {
            return iterator < messages.size();
        });
        while (iterator < messages.size()) {
            last_messages += messages[iterator++] + '\n';
        }
        return std::move(last_messages);
    }
};

#endif //RESTAURANTBES_CHAT_H
