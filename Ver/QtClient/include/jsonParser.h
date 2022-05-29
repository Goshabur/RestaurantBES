#pragma once

#include "MenuList.h"
#include "CartListFWD.h"
#include <memory>
#include <string>
#include <QString>

#include <nlohmann/json.hpp>

namespace restbes {

class JsonParser {
public:
    static MenuItem parseDish(const std::string &input);

    static MenuItem parseDish(const nlohmann::json &json);

    static CartItem parseCartItem(const std::string &input);

    static CartItem parseCartItem(const nlohmann::json &json);

    static std::shared_ptr<MenuData> parseMenu(const std::string &input);

    static std::shared_ptr<MenuData> parseMenu(const nlohmann::json &json);

    static std::shared_ptr<CartData> parseCart(const std::string &input);

    static std::shared_ptr<CartData> parseCart(const nlohmann::json &input);

    static std::string generateRegistrationQuery(const QString &email,
                                                 const QString &password,
                                                 const QString &name,
                                                 const CartList &cartList);

    static nlohmann::json::array_t generateJsonCartData(const CartList &cartList);

    static std::string generateSignInQuery(const QString &email,
                                           const QString &password,
                                           const CartList &cartList);

    static nlohmann::json generateJsonAuthorizationBase(const QString &email,
                                                        const QString &password,
                                                        const CartList &cartList);

    static std::string generateSetItemCountQuery(int dish_id, int count);

    static std::string generateSetCartQuery(const CartList &cartList);

    static std::string generateCreateOrderQuery(const QString &address,
                                                const QString &comment);

    static QString getQStringValue(const nlohmann::json &json,
                                   const char *key);

};

}