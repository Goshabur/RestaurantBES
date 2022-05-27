#pragma once

#include "cart.h"
#include "fwd.h"

using restbes::connectExec;
using restbes::connectGet;

namespace restbes {

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

    std::string create_order();

    [[nodiscard]] std::string cart() const;

    void add_to_cart(id_t dish_id) const noexcept;

    void delete_from_cart(id_t dish_id) const noexcept;

    void empty_cart() const noexcept;

    static std::string show_order_status(id_t order_id);
};

}  // namespace restbes
