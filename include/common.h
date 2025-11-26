// ============================================================================
// FILE: Common.h
// Description: Common enums, constants, and includes used across the system
// ============================================================================

#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <thread>
#include <mutex>
#include <atomic>
#include <chrono>
#include <random>
#include <regex>

// Clearance levels for cards and floors (0-3)
enum class ClearanceLevel {
    LEVEL_0 = 0,  // Lowest clearance
    LEVEL_1 = 1,
    LEVEL_2 = 2,
    LEVEL_3 = 3   // Highest clearance
};

// Access result when trying to enter a floor
struct AccessAttempt {
    std::string employeeId;
    std::string employeeName;
    std::string timestamp;
    bool authorized;
};

// Global constants
const std::string DATA_FILE = "data/users.csv";
const std::string ADMIN_PASSWORD = "Admin@123";  // Hardcoded admin password
const int CACHE_SIZE = 10;

// Utility function to get current timestamp
inline std::string getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

// Convert clearance level to int
inline int clearanceLevelToInt(ClearanceLevel level) {
    return static_cast<int>(level);
}

// Convert int to clearance level
inline ClearanceLevel intToClearanceLevel(int level) {
    return static_cast<ClearanceLevel>(level);
}

#endif // COMMON_H