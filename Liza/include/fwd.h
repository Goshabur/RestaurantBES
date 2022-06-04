#pragma once

#include <ctime>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include "folly/dynamic.h"
#include "folly/json.h"
#include "nlohmann/json.hpp"
#include "pqxx/pqxx"

namespace restbes {

enum OrderStatus { CREATED = 0, ACCEPTED, CANCELLED, IN_PROGRESS, COMPLETED };
enum DishStatus { UNAVAILABLE = 0, AVAILABLE };

static std::vector<std::string> orderStatuses = {
    "CREATED", "ACCEPTED", "CANCELLED", "IN_PROGRESS", "COMPLETED"};

static std::map<std::string, int> orderStatusesMap = {
    {"CREATED", CREATED},
    {"ACCEPTED", ACCEPTED},
    {"CANCELLED", CANCELLED},
    {"IN_PROGRESS", IN_PROGRESS},
    {"COMPLETED", COMPLETED}};

static std::vector<std::string> dishStatuses = {"UNAVAILABLE", "AVAILABLE"};

static std::map<std::string, int> dishStatusesMap = {
    {"UNAVAILABLE", UNAVAILABLE},
    {"AVAILABLE", AVAILABLE}};

static std::ofstream server_request_log("server_request_log.txt",
                                        std::ofstream::app);

static std::ofstream server_error_log("server_error_log.txt",
                                      std::ofstream::app);

std::time_t getTime();

void connectExec(const std::string &sql);

std::string connectGet(const std::string &sql);

pqxx::result connectGet_pqxx_result(const std::string &sql);

}  // namespace restbes
