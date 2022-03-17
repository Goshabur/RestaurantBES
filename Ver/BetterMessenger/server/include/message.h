#ifndef RESTAURANTBES_MESSAGE_H
#define RESTAURANTBES_MESSAGE_H

#include <string>

struct Message {
private:
    const std::string author;
    const std::string message;

public:
    Message(std::string name, std::string text);
    [[nodiscard]] std::string generateMessage() const;
};

#endif //RESTAURANTBES_MESSAGE_H
