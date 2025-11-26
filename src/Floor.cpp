// ============================================================================
// FILE: src/Floor.cpp
// ============================================================================
#include "Floor.h"

Floor::Floor(const std::string& floorId, const std::string& floorName,
             ClearanceLevel clearance)
    : id(floorId), name(floorName), requiredClearance(clearance) {}

std::string Floor::getId() const { return id; }
std::string Floor::getName() const { return name; }
ClearanceLevel Floor::getRequiredClearance() const { return requiredClearance; }
const std::vector<AccessAttempt>& Floor::getAccessHistory() const { return accessHistory; }

void Floor::setName(const std::string& newName) { name = newName; }
void Floor::setRequiredClearance(ClearanceLevel clearance) { requiredClearance = clearance; }

bool Floor::attemptAccess(const User& user) {
    // Check if user's clearance level is sufficient
    bool authorized = user.getCard()->getClearanceLevelInt() >= 
                     clearanceLevelToInt(requiredClearance);
    
    // Log the access attempt
    AccessAttempt attempt;
    attempt.employeeId = user.getId();
    attempt.employeeName = user.getName();
    attempt.timestamp = getCurrentTimestamp();
    attempt.authorized = authorized;
    
    accessHistory.push_back(attempt);
    
    return authorized;
}

void Floor::displayAccessHistory() const {
    std::cout << "\n=== Access History for " << name << " ===" << std::endl;
    std::cout << std::left << std::setw(15) << "Employee ID" 
              << std::setw(25) << "Name" 
              << std::setw(22) << "Timestamp" 
              << "Status" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    
    for (const auto& attempt : accessHistory) {
        std::cout << std::left << std::setw(15) << attempt.employeeId
                  << std::setw(25) << attempt.employeeName
                  << std::setw(22) << attempt.timestamp
                  << (attempt.authorized ? "AUTHORIZED" : "DENIED") << std::endl;
    }
}
