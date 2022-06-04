#include "../../Liza/include/fwd.h"

namespace restbes {

void connectExec(const std::string &sql) {
    pqxx::connection C(
        "dbname=testdb user=postgres password=restbes2022 hostaddr=127.0.0.1 "
        "port=5432");

    pqxx::work W(C);
    W.exec(sql);
    W.commit();

    C.disconnect();
}

std::string connectGet(const std::string &sql) {
    return connectGet_pqxx_result(sql)[0][0].c_str();
}

pqxx::result connectGet_pqxx_result(const std::string &sql) {
    pqxx::connection C(
        "dbname=testdb user=postgres password=restbes2022 hostaddr=127.0.0.1 "
        "port=5432");

    pqxx::nontransaction N(C);
    pqxx::result result(N.exec(sql));
    C.disconnect();

    return result;
}

}  // namespace restbes
