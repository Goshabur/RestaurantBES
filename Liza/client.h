#pragma once

#include "cart.h"
#include "fwd.h"

namespace restbes {

class Client {
private:
    id_t m_id;
    std::string
        m_email;  // TODO: probably unnecessary? We can just take it from DB
    std::string m_name;
    Cart m_cart;

public:
    Client() : m_id(0), m_cart({}){};
    explicit Client(id_t client_id) : m_id(client_id), m_cart({}){};

    explicit Client(const std::string &name,
                    const std::string &email,
                    const std::string &passwd,
                    bool exist)
        : m_name(name), m_email(email) {
        if (exist) {
            m_id = std::stoi(restbes::connect_to_db_get(
                R"(SELECT "CLIENT_ID" FROM "CLIENT" WHERE "EMAIL" = ')" +
                email + "' AND \"PASSWORD\" = crypt('" + passwd +
                "', \"PASSWORD\")"));

            m_cart.client_id = m_id;

        } else {
            m_id = std::stoi(restbes::connect_to_db_get(
                R"(INSERT INTO "CLIENT" ("EMAIL", "NAME", "PASSWORD") VALUES (')" +
                email + "', '" + name + "', " + "crypt('" + passwd +
                "', gen_salt('bf'))) RETURNING \"CLIENT_ID\""));

            restbes::connect_to_db_exec(
                R"(INSERT INTO "CART" ("CLIENT_ID", "COST", "CART") VALUES ()" +
                std::to_string(m_id) + ", 0, '{}')");

            m_cart.client_id = m_id;
        }
    };

    void create_order();

    std::map<std::string, int> cart();

    [[nodiscard]] id_t get_client_id() const;

    void add_to_cart(const std::string &dish_name) noexcept;

    void delete_from_cart(const std::string &dish_name) noexcept;

    void empty_cart() noexcept;
};

}  // namespace restbes
