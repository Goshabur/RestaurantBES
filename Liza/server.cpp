#include <iostream>
#include "client.h"
#include "fwd.h"

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
};

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

    for (auto row : result) {
        if (row[5].as<int>() == 1) {
            std::cout << row[1].as<std::string>() + " -- " << row[3].as<int>()
                      << " rubles\n";
        }
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

}  // namespace restbes

int main() {
    //    restbes::connect_to_db_exec(R"(INSERT INTO "DISH" ("DISH_NAME",
    //    "IMAGE", "PRICE", "STATUS") VALUES ('pizza', 'ff', 970, 1))");
    //    restbes::connect_to_db_exec(R"(INSERT INTO "DISH" ("DISH_NAME",
    //    "IMAGE", "PRICE", "STATUS") VALUES ('soup', 'fg', 620, 1))");
    //    restbes::connect_to_db_exec(R"(INSERT INTO "DISH" ("DISH_NAME",
    //    "IMAGE", "PRICE", "STATUS") VALUES ('cocktail', 'fk', 350, 1))");

    std::string input;
    std::string name;
    std::string password;
    std::string email;
    std::vector<std::string> cart;
    restbes::Client client;

    while (std::getline(std::cin, input)) {
        if (input == "sign_up") {
            std::cout << "Please enter your email: ";
            std::cin >> email;

            if (restbes::check_user_exists(email)) {
                std::cout << "The user with this email already exists, please "
                             "sign in or try again\n";
                continue;
            }

            std::cout << "Your name: ";
            std::cin >> name;
            std::cout << "Create password: ";
            std::cin >> password;

            client = restbes::Client(name, email, password, false);
            std::cout << "Account created, you can now sign in\n";

        } else if (input == "check_sign_in") {
            std::cout << "Enter your email: ";
            std::cin >> email;
            std::cout << "Password: ";
            std::cin >> password;

            if (restbes::check_sign_in(email, password)) {
                name = restbes::connect_to_db_get(
                    R"(SELECT "NAME" FROM "CLIENT" WHERE "EMAIL" = ')" + email +
                    R"(' AND "PASSWORD" = crypt(')" + password +
                    R"(', "PASSWORD"))");

                client = restbes::Client(name, email, password, true);
                std::cout << "Successfully signed in!\n";

            } else {
                std::cout << "The user does not exist, or the password is "
                             "incorrect. Please sign up or try again.\n";
            }

        } else if (input == "show_menu") {
            restbes::show_menu();

        } else if (input == "add_to_cart") {
            std::string product;
            std::cout << "Enter a product name you want to add: ";
            std::cin >> product;

            try {
                std::string tmp = restbes::connect_to_db_get(
                    R"(SELECT "DISH_NAME" FROM "DISH" WHERE "DISH_NAME" = ')" +
                    product + "'");

            } catch (...) {
                std::cout << "Product does not exist, please try again.\n";
                continue;
            }

            client.add_to_cart(product);
            std::cout << "Added to cart.\n";

        } else if (input == "delete_from_cart") {
            std::string product;
            std::cout << "Enter a product name you want to delete: ";
            std::cin >> product;

            try {
                std::string tmp = restbes::connect_to_db_get(
                    R"(SELECT "DISH_NAME" FROM "DISH" WHERE "DISH_NAME" = ')" +
                    product + "'");

            } catch (...) {
                std::cout << "Product does not exist.\n";
                continue;
            }

            client.delete_from_cart(product);
            std::cout << "Deleted from cart.\n";

        } else if (input == "show_cart") {
            client.cart();

        } else if (input == "create_order") {
            client.create_order();
            std::cout
                << "Order created!\nYour order ID is "
                << restbes::connect_to_db_get(
                       R"(SELECT "ORDER_ID" FROM "ACTIVE_ORDER" WHERE "CLIENT_ID" = )" +
                       std::to_string(client.get_client_id()))
                << '\n';

        } else if (input == "clear_cart") {
            client.empty_cart();
            std::cout << "Cart is empty.\n";

        } else if (input == "show_order_status") {
            id_t order_id;
            std::cout << "Enter ORDER_ID: ";
            std::cin >> order_id;

            try {
                std::string status = restbes::connect_to_db_get(
                    R"(SELECT "STATUS" FROM "ORDER" WHERE "ORDER_ID" = )" +
                    std::to_string(order_id));
                std::cout << "Created.\n";  // TODO: get from DB (need to wait
                                            // for admin's change)

            } catch (...) {
                std::cout << "No such order.\n";
            }
        }
    }

    return 0;
}