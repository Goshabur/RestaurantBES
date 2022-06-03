#include "client.h"
#include "order.h"

namespace restbesClient {

std::string Client::get_client_id() const {
    return std::to_string(m_id);
}

std::string Client::cart() const {
    return restbesCart::get_cart(std::to_string(m_id));
}

std::string Client::create_order(const std::string &address,
                                 const std::string &comment) const {
    restbesOrder::Order order(m_id, address, comment);
    restbesCart::set_cart(std::to_string(m_id), "{}", 0);
    restbesOrder::update_order_history(order.get_order_id(), m_id);

    return std::to_string(order.get_order_id());
}

std::string get_client_name(const std::string &client_id) {
    return connectGet(R"(SELECT "NAME" FROM "CLIENT" WHERE "CLIENT_ID" = )" +
                      client_id);
}

std::string get_client_email(const std::string &client_id) {
    return connectGet(R"(SELECT "EMAIL" FROM "CLIENT" WHERE "CLIENT_ID" = )" +
                      client_id);
}

std::string get_client_cart(const std::string &client_id) {
    return connectGet(
        R"(SELECT "CART"::TEXT FROM "CLIENT" WHERE "CLIENT_ID" = )" +
        client_id);
}

std::string get_client_id_by_email(const std::string &email) {
    return connectGet(R"(SELECT "CLIENT_ID" FROM "CLIENT" WHERE "EMAIL" = ')" +
                      email + "'");
}

bool check_user_exists(const std::string &email) {
    return !restbes::connectGet_pqxx_result(
                R"(SELECT "CLIENT_ID" FROM "CLIENT" WHERE "EMAIL" = ')" +
                email + "'")
                .empty();
}

bool check_sign_in(const std::string &email, const std::string &password) {
    return !restbes::connectGet_pqxx_result(
                R"(SELECT "CLIENT_ID" FROM "CLIENT" WHERE "EMAIL" = ')" +
                email + "' AND \"PASSWORD\" = crypt('" + password +
                "', \"PASSWORD\")")
                .empty();
}

}  // namespace restbesClient
