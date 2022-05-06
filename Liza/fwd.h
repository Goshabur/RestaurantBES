#pragma once

#include <fstream>
#include <string>
#include <vector>
#include "pqxx/pqxx"

namespace restbes {

namespace {
enum OrderStatus { CREATED = 0, ACCEPTED, CANCELLED, IN_PROGRESS, COMPLETED };

[[maybe_unused]] std::vector<std::string> Statuses = {
    "CREATED", "ACCEPTED", "CANCELLED", "IN_PROGRESS", "COMPLETED"};

std::ofstream server_request_log("server_request_log.txt", std::ofstream::app);
std::ofstream server_error_log("server_error_log.txt", std::ofstream::app);
}  // namespace

void connectExec(const std::string &sql);

std::string connectGet(const std::string &sql);

}  // namespace restbes
