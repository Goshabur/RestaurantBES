#pragma once

#include "fwd.h"

#include <restbed>
#include <folly/Synchronized.h>

#include <memory>
#include <queue>

namespace restbes {

struct Session {
private:
    std::shared_ptr<restbed::Session> session;
    folly::Synchronized<std::string> user_id;
    folly::Synchronized<unsigned int> session_id;
    folly::Synchronized<std::queue<std::shared_ptr<restbed::Response>>> responseQueue;

    void yieldFromQueue();

public:
    Session(std::shared_ptr<restbed::Session> ss, std::string uid);

    void setUser(std::string uid);

    void setSession(std::shared_ptr<restbed::Session> ss);

    [[nodiscard]] bool is_open() const;

    [[nodiscard]] bool is_closed() const;

    [[nodiscard]] std::string getPath() const;

    void push(std::shared_ptr<restbed::Response> response);

    [[nodiscard]] std::string getUserId() const;

    [[nodiscard]] std::shared_ptr<restbed::Session> getSession() const;

    [[nodiscard]] unsigned int getId() const;


};

} // namespace restbes
