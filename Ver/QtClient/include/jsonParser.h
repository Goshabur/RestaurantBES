#pragma once

#include "MenuList.h"
#include <memory>
#include <string>
#include <QString>

#include <nlohmann/json.hpp>

namespace restbes {

class JsonParser {
public:
    static MenuItem parseDish(const std::string &input);

    static MenuItem parseDish(const nlohmann::json &json);

    static std::shared_ptr<MenuData> parseMenu(const std::string &input);

    static std::shared_ptr<MenuData> parseMenu(const nlohmann::json &json);

    // Type 1
    static std::string generateCreateOrderQuery(int user_id);

    static std::string generateEmptyCartQuery(int user_id);

    static std::string generateShowCartQuery(int user_id);

    // Type 2
    static std::string generateAddToCartQuery(int user_id, int product_id);

    static std::string generateDeleteFromCartQuery(int user_id, int product_id);

    // Type 3
    static std::string generateRegistrationQuery(const QString &email,
                                                 const QString &password,
                                                 const QString &name);

    static std::string generateSignInQuery(const QString &email,
                                           const QString &password);

    static QString getQStringValue(const nlohmann::json &json,
                                   const char *key);


// TODO:
//    std::shared_ptr<CartList> parseCartList(json input);
//    std::shared_ptr<Client> parseClient(json input);

private:


};

}