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

static std::time_t time_now = std::time(nullptr);

enum OrderStatus { CREATED = 0, ACCEPTED, CANCELLED, IN_PROGRESS, COMPLETED };
enum DishStatus { UNAVAILABLE = 0, AVAILABLE };

static std::vector<std::string> orderStatuses = {"CREATED", "ACCEPTED", "CANCELLED",
                                          "IN_PROGRESS", "COMPLETED"};

static std::map<std::string, int> orderStatusesMap = {{"CREATED", 0},
                                               {"ACCEPTED", 1},
                                               {"CANCELLED", 2},
                                               {"IN_PROGRESS", 3},
                                               {"COMPLETED", 4}};

static std::vector<std::string> dishStatuses = {"UNAVAILABLE", "AVAILABLE"};

static std::map<std::string, int> dishStatusesMap = {{"UNAVAILABLE", 0},
                                              {"AVAILABLE", 1}};

static std::ofstream server_request_log("server_request_log.txt", std::ofstream::app);

static std::ofstream server_error_log("server_error_log.txt", std::ofstream::app);

void connectExec(const std::string &sql);

std::string connectGet(const std::string &sql);

pqxx::result connectGet_pqxx_result(const std::string &sql);

}  // namespace restbes
