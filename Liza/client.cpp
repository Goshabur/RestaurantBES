#include "client.h"
#include "order.h"
#include "admin.h"

namespace client {
    void Client::create_order() const {
        // Пользователь нажимает кнопку "оформить заказ":
        // Создаем объект класса "Order", заносим в БД сразу в конструкторе (нужно получить корзину...)
        // Также заносим изменения в БД пользователя, что у него появился новый заказ
        // Админ получает сигнал, что поступил заказ, подтверждает его (меняет статус заказа) = вносит изменения в БД.

        order::Order y(cart);

        db::connect_to_db_exec(
                "INSERT INTO CLIENT (ID, EMAIL, NAME, ACTIVE_ORDER) VALUES (" + std::to_string(id) + ", " + email +
                ", " + name + ", " + std::to_string(y.id) + ")");

        // TODO: signal to administrator so they could accept the order
        // administrator.accept_order(*this);
    }

    void Client::get_dish_info(int dish_id) {
        /* TODO: transfer info (how?) (На Гоше?) */
    }

    void Client::add_dish_to_cart(int dish_id) {
        cart.emplace_back(dish_id);
        // TODO: вынести в клиент?
    }

    void Client::view_cart() {
        /* TODO: show cart (Гоша?) */
    }

    std::string Client::get_order_status() const {
        return db::connect_to_db_get("SELECT STATUS FROM ORDER WHERE ID = '" + std::to_string(is_active_order) + "'");
        // TODO: это можно либо всё хранить в БД, либо оставить какие-то поля у объекта класса "Order" и просто обращаться к ним
    }


}
