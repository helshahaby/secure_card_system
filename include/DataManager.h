// ============================================================================
// FILE: DataManager.h
// Description: Handles CSV persistence and data operations
// ============================================================================

#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include "common.h"
#include "User.h"
#include "Admin.h"
#include "Floor.h"

class DataManager {
private:
    std::mutex dataMutex;  // Mutex for thread-safe operations

public:
    // Load users and admin from CSV file
    void loadFromCSV(std::vector<std::shared_ptr<User>>& users,
                     std::shared_ptr<Admin>& admin);

    // Save users and admin to CSV file (thread-safe)
    void saveToCSV(const std::vector<std::shared_ptr<User>>& users,
                   const std::shared_ptr<Admin>& admin);

    // Generate unique employee ID with suffix if needed
    std::string generateUniqueId(const std::string& baseName,
                                  const std::vector<std::shared_ptr<User>>& users);

    // Parse CSV line
    std::vector<std::string> parseCSVLine(const std::string& line);

    // Escape CSV field like like commas, newlines, and double quotes
    std::string escapeCSV(const std::string& field);
};

#endif // DATAMANAGER_H