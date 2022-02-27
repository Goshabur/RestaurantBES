#include <pqxx/pqxx>
#include "server.h"
#include "client.h"
#include "admin.h"
#include <iostream>

namespace db {
    void connect_to_db_exec(const std::string &sql) {
        // Устанавливаем соединение с БД
        pqxx::connection C("dbname = testdb user = postgres password = restbes2022 hostaddr = 127.0.0.1 port = 5432");

        if (C.is_open()) {
            std::cout << "Opened database successfully: " << C.dbname() << std::endl;
        } else {
            std::cout << "Can't open database" << std::endl;
            return;
        }

        // Transactional object: с помощью него мы можем выполнять SQL statements (менять что-то в БД)
        pqxx::work W(C);

        W.exec(sql);
        W.commit();

        // TODO: signal that something has changed

        C.disconnect();
    };

    std::string connect_to_db_get(const std::string &sql) {
        pqxx::connection C("dbname = testdb user = postgres password = restbes2022 hostaddr = 127.0.0.1 port = 5432");

        // Nontransactional object: не меняет, лишь достаёт данные
        pqxx::nontransaction N(C);

        pqxx::result result(N.exec(sql));
        pqxx::const_result_iterator c = result.begin();

        C.disconnect();

        return c[0].as<std::string>();
    }
}  // namespace db

int main() {
    admin::Admin administrator;

    client::Client user("Lisa", "example@mail.ru");
    user.cart.emplace_back(235);
    user.create_order();

    std::cout << user.get_order_status() << std::endl;

    administrator.cancel_order(1);

    std::cout << user.get_order_status() << std::endl;

    return 0;
}
