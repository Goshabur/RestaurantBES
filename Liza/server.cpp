#include "fwd.h"
#include <iostream>
#include "admin.h"
#include "client.h"

namespace restbes {
    void connect_to_db_exec(const std::string &sql) {
        pqxx::connection C("dbname=testdb user=postgres password=restbes2022 hostaddr=127.0.0.1 port=5432");

//        if (C.is_open()) {
//            std::cout << "Opened database successfully: " << C.dbname() << std::endl;
//        } else {
//            std::cout << "Can't open database" << std::endl;
//            return;
//        }

        pqxx::work W(C);

        W.exec(sql);
        W.commit();

        // TODO: signal that something has changed and/or executed successfully

        C.disconnect();
    };

    std::string connect_to_db_get(const std::string &sql) {
        pqxx::connection C("dbname=testdb user=postgres password=restbes2022 hostaddr=127.0.0.1 port=5432");

        pqxx::nontransaction N(C);

        pqxx::result result(N.exec(sql));

        C.disconnect();

        return result[0][0].c_str();
    }
}  // namespace restbes

int main() {
    restbes::Client user("Lisa", "example@mail.ru");

    user.add_to_cart(1);
    user.add_to_cart(2);
    user.delete_from_cart(1);

    user.create_order();

    restbes::Admin::accept_order(user);

    std::cout << user.get_order_status() << std::endl;

    restbes::Admin::cancel_order(std::stoi(user.get_active_order()));

    std::cout << user.get_order_status() << std::endl;

    return 0;
}