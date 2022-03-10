#pragma once

#include "fwd.h"
#include "cart.h"

namespace restbes {
    class Client {
    private:
        int m_id;
        std::string m_email;
        std::string m_name;
        int m_active_order = 0;

    public:
        Cart cart;

        explicit Client(const std::string &name, const std::string &email) : m_name(name), m_email(email) {

            // TODO: check for valid m_email address

            m_id = std::stoi(restbes::connect_to_db_get(
                    R"(INSERT INTO "CLIENT" ("EMAIL", "NAME", "ACTIVE_ORDER") VALUES (')" + email +
                    "', '" + name + "', " + "0) RETURNING \"ID\""));

        };

        [[nodiscard]] std::string get_active_order() const;

        [[nodiscard]] std::string get_order_status() const;

        void create_order();

        void add_to_cart(int dish_id) noexcept;

        void delete_from_cart(int dish_id) noexcept;

        // void get_dish_info(int dish_id);

        // void add_dish_to_cart(int dish_id);

        // void view_cart();
    };
}  // namespace restbes
