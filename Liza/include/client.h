#pragma once

#include "cart.h"
#include "fwd.h"

using restbes::connectExec;
using restbes::connectGet;
using restbesCart::Cart;

namespace restbesClient {

class Client {
private:
    id_t m_id;
    std::string m_email;
    std::string m_name;
    Cart m_cart;

public:
    Client() : m_id(0), m_cart(){};

    explicit Client(id_t client_id) : m_id(client_id), m_cart(){};

    explicit Client(const std::string &name,
                    const std::string &email,
                    const std::string &password,
                    const std::string &cart)
        : m_email(email), m_name(name), m_cart() {
        m_id = std::stoi(connectGet(
            R"(INSERT INTO "CLIENT" ("EMAIL", "NAME", "PASSWORD") VALUES (')" +
            email + "', '" + name + "', " + "crypt('" + password +
            "', gen_salt('bf'))) RETURNING \"CLIENT_ID\""));

        m_cart = Cart(m_id, cart);
    };

    [[nodiscard]] std::string get_client_id() const;

    [[nodiscard]] std::string cart() const;

    [[nodiscard]] std::string create_order(const std::string &address,
                                           const std::string &comment) const;
};

std::string get_client_name(const std::string &client_id);

std::string get_client_email(const std::string &client_id);

std::string get_client_cart(const std::string &client_id);

std::string get_client_id_by_email(const std::string &email);

bool check_user_exists(const std::string &email);

bool check_sign_in(const std::string &email, const std::string &password);

}  // namespace restbesClient
