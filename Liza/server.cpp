#include "server.h"
#include <fstream>
#include <iostream>
#include <restbed>
#include "admin.h"
#include "client.h"
#include "fwd.h"
#include "order.h"
#include "server/include/fwd.h"
#include "user.h"

using restbed::Session;
using server_structure::Connection;
using server_structure::Server;

namespace restbes {

void connect_to_db_exec(const std::string &sql) {
    pqxx::connection C(
        "dbname=testdb user=postgres password=restbes2022 hostaddr=127.0.0.1 "
        "port=5432");

    pqxx::work W(C);
    W.exec(sql);
    W.commit();

    // TODO: signal that something has changed and/or executed successfully

    C.disconnect();
}

std::string connect_to_db_get(const std::string &sql) {
    pqxx::connection C(
        "dbname=testdb user=postgres password=restbes2022 hostaddr=127.0.0.1 "
        "port=5432");

    pqxx::nontransaction N(C);
    pqxx::result result(N.exec(sql));

    C.disconnect();

    return result[0][0].c_str();
}

void show_menu() {
    std::string sql = "SELECT * FROM \"DISH\"";
    pqxx::connection C(
        "dbname=testdb user=postgres password=restbes2022 hostaddr=127.0.0.1 "
        "port=5432");

    pqxx::work W(C);
    pqxx::result result(W.exec(sql));

    C.disconnect();

    // console-mode only. TODO
    for (auto row : result) {
        if (row[5].as<int>() == 1) {
            std::cout << row[1].as<std::string>() + " -- " << row[3].as<int>()
                      << " rubles\n";
        }
    }
}

void show_order_status(id_t order_id) {
    try {
        std::string status = restbes::connect_to_db_get(
            R"(SELECT "STATUS" FROM "ORDER" WHERE "ORDER_ID" = )" +
            std::to_string(order_id));

        std::cout << (status == "1"
                          ? "Accepted\n"
                          : (status == "0" ? "Created\n" : "Cancelled\n"));

    } catch (...) {
        std::cout << "No such order.\n";
    }
}

bool check_user_exists(const std::string &email) {
    try {
        std::string tmp = restbes::connect_to_db_get(
            R"(SELECT "CLIENT_ID" FROM "CLIENT" WHERE "EMAIL" = ')" + email +
            "'");

    } catch (...) {
        return false;
    }

    return true;
}

bool check_sign_in(const std::string &email, const std::string &password) {
    try {
        std::string tmp = restbes::connect_to_db_get(
            R"(SELECT "CLIENT_ID" FROM "CLIENT" WHERE "EMAIL" = ')" + email +
            "' AND \"PASSWORD\" = crypt('" + password + "', \"PASSWORD\")");

    } catch (...) {
        return false;
    }

    return true;
}

bool check_admin(const std::string &password) {
    try {
        std::string tmp = restbes::connect_to_db_get(
            R"(SELECT "ADMIN_ID" FROM "ADMINISTRATOR" WHERE "PASSWORD" = crypt(')" +
            password + "', \"PASSWORD\")");

    } catch (...) {
        return false;
    }

    return true;
}

void getMethodHandler(std::shared_ptr<Session> session,
                      std::shared_ptr<Server> server) {
    std::string name = session->get_request()->get_header("Name");

    if (server->getUser(name) == nullptr) {
        server->addUser(name);
    }

    auto user = server->getUser(name);

    if (user->getSession(session) == nullptr) {
        user->addSession(session);
    }
}

// TODO:
void postAdminMethodHandler(std::shared_ptr<Session> session,
                            const std::string &data,
                            std::shared_ptr<Server> server) {
    auto request = session->get_request();
    std::string name = request->get_header("admin_id");

    nlohmann::json data_ = data;
    std::string command_ = data_.at("command");
    std::string password_ = data_.at("password");

    if (!check_admin(password_)) {
        std::string response_body = "Failed to authorize\n";
        auto response = generateResponse(response_body, Connection::CLOSE);
        session->close(*response);
        return;
    }

    id_t admin_id_ = static_cast<id_t>(data_.at("admin_id"));
    id_t order_id_ = static_cast<id_t>(data_.at("order_id"));
    id_t product_id_ = static_cast<id_t>(data_.at("product_id"));
    int32_t value_to_set_ = static_cast<int32_t>(data_.at("value_to_set"));

    // TODO: admin wants to send requests, not responses

    // make const, send JSONs
    std::string response_body = "PROCESSED, OK\n";
    auto response = generateResponse(response_body, Connection::CLOSE);
    session->close(*response);
}

void postCartMethodHandler(std::shared_ptr<Session> session,
                           const std::string &data,
                           std::shared_ptr<Server> server) {
    auto request = session->get_request();
    std::string name = request->get_header("client_id_");

    nlohmann::json data_ = data;
    std::string command_ = data_.at("command");
    id_t client_id_ = static_cast<id_t>(data_.at("client_id_"));
    id_t product_id_ = static_cast<id_t>(data_.at("product_id_"));

    Client client = Client(client_id_);
    if (command_ == "add_to_cart") {
        client.add_to_cart(product_id_);
        server->getUser(name)->yieldMessage(
            "Added to cart: " + std::to_string(product_id_) + '\n',
            request->get_path());

    } else if (command_ == "delete_from_cart") {
        client.delete_from_cart(product_id_);
        server->getUser(name)->yieldMessage(
            "Deleted from cart: " + std::to_string(product_id_) + '\n',
            request->get_path());

    } else if (command_ == "clear_cart") {
        client.empty_cart();
        server->getUser(name)->yieldMessage("Cart is empty\n",
                                            request->get_path());

    } else if (command_ == "show_cart") {
        client.cart();  // TODO: send request to Gosha? Send json?
    }

    // make const, send JSONs
    std::string response_body = "PROCESSED, OK\n";
    auto response = generateResponse(response_body, Connection::CLOSE);
    session->close(*response);
}

void postOrderMethodHandler(std::shared_ptr<Session> session,
                            const std::string &data,
                            std::shared_ptr<Server> server) {
    auto request = session->get_request();
    std::string name = request->get_header("Name");

    nlohmann::json data_ = data;
    std::string command_ = data_.at("command");
    id_t client_id_ = static_cast<id_t>(data_.at("client_id_"));
    id_t order_id_ = static_cast<id_t>(data_.at("order_id_"));

    Client client = Client(client_id_);
    if (command_ == "create_order") {
        client.create_order();

    } else if (command_ == "show_order_status") {
        show_order_status(order_id_);
    }

    // make const, send JSONs
    std::string response_body = "PROCESSED, OK\n";
    auto response = generateResponse(response_body, Connection::CLOSE);
    session->close(*response);
}

}  // namespace restbes

// int main() {
//     restbes::connect_to_db_exec(R"(INSERT INTO "DISH" ("DISH_NAME",
//     "IMAGE", "PRICE", "STATUS") VALUES ('pizza', 'ff', 970, 1))");
//     restbes::connect_to_db_exec(R"(INSERT INTO "DISH" ("DISH_NAME",
//     "IMAGE", "PRICE", "STATUS") VALUES ('soup', 'fg', 620, 1))");
//     restbes::connect_to_db_exec(R"(INSERT INTO "DISH" ("DISH_NAME",
//     "IMAGE", "PRICE", "STATUS") VALUES ('cocktail', 'fk', 350, 1))");
//
//    std::string input;
//    std::string name;
//    std::string password;
//    std::string email;
//    std::vector<std::string> cart;
//    restbes::Client client;
//
//    while (std::getline(std::cin, input)) {
//        if (input == "sign_up") {
//            std::cout << "Please enter your email: ";
//            std::cin >> email;
//
//            if (restbes::check_user_exists(email)) {
//                std::cout << "The user with this email already exists, please
//                "
//                             "sign in or try again\n";
//                continue;
//            }
//
//            std::cout << "Your name: ";
//            std::cin >> name;
//            std::cout << "Create password: ";
//            std::cin >> password;
//
//            client = restbes::Client(name, email, password, false);
//            std::cout << "Account created, you can now sign in\n";
//
//        } else if (input == "sign_in") {
//            std::cout << "Enter your email: ";
//            std::cin >> email;
//            std::cout << "Password: ";
//            std::cin >> password;
//
//            if (restbes::check_sign_in(email, password)) {
//                name = restbes::connect_to_db_get(
//                    R"(SELECT "NAME" FROM "CLIENT" WHERE "EMAIL" = ')" + email
//                    + R"(' AND "PASSWORD" = crypt(')" + password + R"(',
//                    "PASSWORD"))");
//
//                client = restbes::Client(name, email, password, true);
//                std::cout << "Successfully signed in!\n";
//
//            } else {
//                std::cout << "The user does not exist, or the password is "
//                             "incorrect. Please sign up or try again.\n";
//            }
//
//        } else if (input == "show_menu") {
//            restbes::show_menu();
//
//        } else if (input == "add_to_cart") {
//            std::string product;
//            std::cout << "Enter a product name you want to add: ";
//            std::cin >> product;
//
//            try {
//                std::string tmp = restbes::connect_to_db_get(
//                    R"(SELECT "DISH_NAME" FROM "DISH" WHERE "DISH_NAME" = ')"
//                    + product + "'");
//
//            } catch (...) {
//                std::cout << "Product does not exist, please try again.\n";
//                continue;
//            }
//
//            client.add_to_cart(product);
//            std::cout << "Added to cart.\n";
//
//        } else if (input == "delete_from_cart") {
//            std::string product;
//            std::cout << "Enter a product name you want to delete: ";
//            std::cin >> product;
//
//            try {
//                std::string tmp = restbes::connect_to_db_get(
//                    R"(SELECT "DISH_NAME" FROM "DISH" WHERE "DISH_NAME" = ')"
//                    + product + "'");
//
//            } catch (...) {
//                std::cout << "Product does not exist.\n";
//                continue;
//            }
//
//            client.delete_from_cart(product);
//            std::cout << "Deleted from cart.\n";
//
//        } else if (input == "show_cart") {
//            client.cart();
//
//        } else if (input == "create_order") {
//            client.create_order();
//            std::cout
//                << "Order created!\nYour order ID is "
//                << restbes::connect_to_db_get(
//                       R"(SELECT "ORDER_ID" FROM "ACTIVE_ORDER" WHERE
//                       "CLIENT_ID" = )" +
//                       std::to_string(client.get_client_id()))
//                << '\n';
//
//        } else if (input == "clear_cart") {
//            client.empty_cart();
//            std::cout << "Cart is empty.\n";
//
//        } else if (input == "show_order_status") {
//            id_t order_id;
//            std::cout << "Enter order id: ";
//            std::cin >> order_id;
//
//            restbes::show_order_status(order_id);
//
//        } else if (input == "log_in_as_admin") {
//            std::cout << "Password: ";
//            std::cin >> password;
//
//            if (restbes::check_admin(password)) {
//                std::cout << "Success.\n";
//
//            } else {
//                std::cout << "Denied.\n";
//                continue;
//            }
//
//            while (std::getline(std::cin, input)) {
//                id_t id;
//                int new_price;
//
//                if (input == "accept_order") {
//                    std::cout << "Order id: ";
//                    std::cin >> id;
//                    restbes::Admin::accept_order(id);
//
//                    std::cout << "Successfully changed\n";
//
//                } else if (input == "cancel_order") {
//                    std::cout << "Order id: ";
//                    std::cin >> id;
//                    restbes::Admin::cancel_order(id);
//
//                    std::cout << "Successfully changed\n";
//
//                } else if (input == "change_dish_price") {
//                    std::cout << "Dish name: ";
//                    std::cin >> name;
//                    std::cout << "New price: ";
//                    std::cin >> new_price;
//                    restbes::Admin::change_dish_price(name, new_price);
//
//                    std::cout << "Successfully changed\n";
//                }
//            }
//        }
//    }
//
//    return 0;
//}
