// ============================================================================
// FILE: Floor.h
// Description: Floor class - represents a floor with access control
// ============================================================================

#ifndef FLOOR_H
#define FLOOR_H

#include "common.h"
#include "User.h"

class Floor {
private:
    std::string id;                          // Unique floor ID
    std::string name;                        // Unique floor name
    ClearanceLevel requiredClearance;        // Required clearance level
    std::vector<AccessAttempt> accessHistory; // Access log (runtime only)

public:
    // Constructor
    Floor(const std::string& floorId, const std::string& floorName,
          ClearanceLevel clearance);

    // Getters
    std::string getId() const;
    std::string getName() const;
    ClearanceLevel getRequiredClearance() const;
    const std::vector<AccessAttempt>& getAccessHistory() const;

    // Setters
    void setName(const std::string& newName);
    void setRequiredClearance(ClearanceLevel clearance);

    // Access control - returns true if user is authorized
    bool attemptAccess(const User& user);

    // Display access history
    void displayAccessHistory() const;
};

#endif // FLOOR_H