#include "jsonParser.h"
#include "MenuItem.h"
#include "MenuList.h"

#include <iostream>

namespace restbes {


MenuItem JsonParser::parseDish(const std::string &input) {
    nlohmann::json jsonDish = nlohmann::json::parse(input);
    return std::move(parseDish(jsonDish));
}

MenuItem JsonParser::parseDish(const nlohmann::json &json) {
    return std::move(
            MenuItem{
                    json.at("id"),
                    getQStringValue(json, "name"),
                    getQStringValue(json, "image"),
                    json.at("price"),
                    getQStringValue(json, "info"),
                    json.at("status")
            });
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

QString
JsonParser::getQStringValue(const nlohmann::json &json, const char *key) {
    return {json.at(key).get<std::string>().c_str()};
}

std::string JsonParser::generateCreateOrderQuery(int user_id) {
    nlohmann::json json({
                                {"command", "create_order"},
                                {"user_id", user_id}
                        });
    return json.dump();
}

std::string JsonParser::generateEmptyCartQuery(int user_id) {
    nlohmann::json json({
                                {"command", "empty_cart"},
                                {"user_id", user_id}
                        });
    return json.dump();
}

std::string JsonParser::generateShowCartQuery(int user_id) {
    nlohmann::json json({
                                {"command", "show_cart"},
                                {"user_id", user_id}
                        });
    return json.dump();
}

std::string JsonParser::generateAddToCartQuery(int user_id, int product_id) {
    nlohmann::json json({
                                {"command",    "add_to_cart"},
                                {"user_id",    user_id},
                                {"product_id", product_id}
                        });
    return json.dump();
}

std::string
JsonParser::generateDeleteFromCartQuery(int user_id, int product_id) {
    nlohmann::json json({
                                {"command",    "delete_from_cart"},
                                {"user_id",    user_id},
                                {"product_id", product_id}
                        });
    return json.dump();
}

std::string JsonParser::generateRegistrationQuery(const QString &email,
                                                  const QString &password,
                                                  const QString &name) {
    nlohmann::json json({
                                {"command",  "sign_up"},
                                {"email",    email.toStdString()},
                                {"password", password.toStdString()},
                                {"name",     name.toStdString()}
                        });
    return json.dump();
}

std::string
JsonParser::generateSignInQuery(const QString &email, const QString &password) {
    nlohmann::json json({
                                {"command",  "sign_in"},
                                {"email",    email.toStdString()},
                                {"password", password.toStdString()}
                        });
    return json.dump();
}

}