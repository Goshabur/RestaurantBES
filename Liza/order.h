#pragma once

#include "fwd.h"

namespace restbes {
    class Order {
    private:
        int m_id;
        int m_status = 0; /* created/accepted/cancelled/in progress/completed 0/1/2/3/4 */

    public:
        explicit Order(Cart &obj) {
            m_id = std::stoi(restbes::connect_to_db_get(R"(INSERT INTO "ORDER" ("STATUS") VALUES ()" +
                                                        std::to_string(m_status) + ") RETURNING \"ID\""));

            std::string jsonb = "[";
            for (auto item: obj.get_cart()) {
                jsonb += "{\"dish_id\" : " + std::to_string(item.first) + ", \"quantity\" : " +
                         std::to_string(item.second) + "}, ";
            }
            jsonb.erase(jsonb.length() - 2);  //  remove ', '
            jsonb += "]";

            restbes::connect_to_db_exec(
                    R"(UPDATE "ORDER" set "ITEMS" = ')" + jsonb + "' where \"ID\"=" +
                    std::to_string(m_id));

        }

        [[nodiscard]] int get_order_id() const noexcept {
            return m_id;
        }
    };
}  // namespace restbes
