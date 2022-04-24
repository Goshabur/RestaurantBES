#pragma once

#include <fstream>
#include <string>
#include <vector>
#include "pqxx/pqxx"

namespace restbes {

namespace {
std::ofstream server_request_log("server_request_log.txt", std::ofstream::app);
std::ofstream server_error_log("server_error_log.txt", std::ofstream::app);

const std::string request = "REQUEST: ";
const std::string error = "ERROR: ";
}  // namespace

void connect_to_db_exec(const std::string &sql);

std::string connect_to_db_get(const std::string &sql);

}  // namespace restbes
