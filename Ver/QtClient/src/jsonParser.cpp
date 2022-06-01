#include "jsonParser.h"
#include "MenuItem.h"
#include "MenuList.h"
#include "CartList.h"

#include <iostream>

namespace restbes {


MenuItem JsonParser::parseDish(const std::string &input) {
    nlohmann::json jsonDish = nlohmann::json::parse(input);
    return std::move(parseDish(jsonDish));
}

MenuItem JsonParser::parseDish(const nlohmann::json &json) {
    return {
            json.at("id"),
            getQStringValue(json, "name"),
            getQStringValue(json, "image"),
            json.at("price"),
            getQStringValue(json, "info"),
            json.at("status")
    };
}

CartItem JsonParser::parseCartItem(const std::string &input) {
    nlohmann::json jsonCartItem = nlohmann::json::parse(input);
    return parseCartItem(jsonCartItem);
}

CartItem JsonParser::parseCartItem(const nlohmann::json &json) {
    return {
            json.at("id"),
            json.at("quantity")
    };
}

std::shared_ptr<MenuData> JsonParser::parseMenu(const std::string &input) {
    nlohmann::json jsonMenu = nlohmann::json::parse(input);
    return parseMenu(jsonMenu);
}

std::shared_ptr<MenuData> JsonParser::parseMenu(const nlohmann::json &json) {
    auto menuData = std::make_shared<MenuData>();
    const nlohmann::json::array_t &dishArray = json.at("contents");
    for (const auto &dish: dishArray) {
        auto dishItem = parseDish(dish);
        menuData->insert({dishItem.id, std::move(dishItem)});
    }
    return menuData;
}

std::shared_ptr<CartData> JsonParser::parseCart(const std::string &input) {
    nlohmann::json jsonCart = nlohmann::json::parse(input);
    return parseCart(jsonCart);
}

std::shared_ptr<CartData> JsonParser::parseCart(const nlohmann::json &json) {
    auto cartData = std::make_shared<CartData>();
    const nlohmann::json::array_t &cartArray = json.at("contents");
    for (const auto &item: cartArray) {
        cartData->push_back(parseCartItem(item));
    }
    return cartData;
}

QString
JsonParser::getQStringValue(const nlohmann::json &json, const char *key) {
    return {json.at(key).get<std::string>().c_str()};
}

std::string JsonParser::generateCreateOrderQuery(const QString &address,
                                                 const QString &comment) {
    nlohmann::json
            json({
                         {"query", "create_order"},
                         {"body",  {
                                           {"address", address.toStdString()},
                                           {"comment", comment.toStdString()}
                                   }}
                 });
    return json.dump();
}

std::string JsonParser::generateSetCartQuery(const CartList &cartList) {
    nlohmann::json json({
                                {"query", "set_cart"}
                        });
    json["body"]["cart"] = generateJsonCartData(cartList);
    return json.dump();
}

std::string JsonParser::generateSetItemCountQuery(int dish_id, int count) {
    nlohmann::json
            json({
                         {"query", "set_item_count"},
                         {"body",  {
                                           {"dish_id", dish_id},
                                           {"count", count}
                                   }
                         }
                 });
    return json.dump();
}

std::string JsonParser::generateRegistrationQuery(const QString &email,
                                                  const QString &password,
                                                  const QString &name,
                                                  const CartList &cartList) {
    nlohmann::json json = generateJsonAuthorizationBase(email, password,
                                                        cartList);
    json["query"] = "sign_up";
    json["body"]["name"] = name.toStdString();
    return json.dump();
}

std::string
JsonParser::generateSignInQuery(const QString &email,
                                const QString &password,
                                const CartList &cartList) {
    nlohmann::json json = generateJsonAuthorizationBase(email, password,
                                                        cartList);
    json["query"] = "sign_in";
    return json.dump();
}

nlohmann::json::array_t
JsonParser::generateJsonCartData(const CartList &cartList) {
    nlohmann::json::array_t cart;
    for (int i = 0; i < cartList.size(); ++i) {
        CartItem cartItem = cartList.getItemAt(i);
        cart.push_back({
                               {"dish_id", cartItem.item_id},
                               {"count",   cartItem.count}
                       });
    }
    return std::move(cart);
}

nlohmann::json JsonParser::generateJsonAuthorizationBase(const QString &email,
                                                         const QString &password,
                                                         const CartList &cartList) {
    nlohmann::json
            json({
                         {"body", {
                                 {"email", email.toStdString()},
                                 {"password", password.toStdString()}
                         }
                         }
                 });
    auto &body = json["body"];
    bool update_cart = cartList.size() != 0;
    body["update_cart"] = update_cart;
    if (update_cart) {
        body["cart"] = generateJsonCartData(cartList);
    }
    return json;
}

void JsonParser::parseOrder(const std::string &input, Order &order) {
    nlohmann::json json = nlohmann::json::parse(input);
    parseOrder(json, order);
}

void JsonParser::parseOrder(const nlohmann::json &input, Order &order) {
    order.setOrderId(input["order_id"].get<int>());
    order.setTimestamp(input["timestamp"].get<uint32_t>());
    order.setLastModified(input["last_modified"].get<uint32_t>());
    order.setTotalCost(input["cost"].get<int>());
    order.setStatus(input["status"].get<int>());
    order.setAddress(getQStringValue(input, "address"));
    order.setComment(getQStringValue(input, "comment"));
    auto *orderCart = new CartList();
    orderCart->setCart(parseCart(input["cart"]));
    order.setCart(orderCart);
}

std::shared_ptr<OrderData>
JsonParser::parseOrderData(const std::string &input) {
    nlohmann::json::array_t json = nlohmann::json::parse(input);
    return parseOrderData(json);
}

std::shared_ptr<OrderData>
JsonParser::parseOrderData(const nlohmann::json &input) {
    auto orderData = std::make_shared<OrderData>();
    for (const auto &order: input) {
        orderData->push_back({order["order_id"].get<int>(),
                              order["status"].get<int>(),
                              order["timestamp"].get<uint32_t>()
                             });
    }
    return orderData;
}

}