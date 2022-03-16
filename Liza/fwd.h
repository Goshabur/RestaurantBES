#pragma once

#include <string>
#include <vector>
#include "pqxx/pqxx"

namespace restbes {

void connect_to_db_exec(const std::string &sql);

std::string connect_to_db_get(const std::string &sql);

}  // namespace restbes
