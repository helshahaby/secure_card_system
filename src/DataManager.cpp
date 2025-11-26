// ============================================================================
// FILE: src/DataManager.cpp
// ============================================================================

#include "DataManager.h"

void DataManager::loadFromCSV(std::vector<std::shared_ptr<User>>& users,
                               std::shared_ptr<Admin>& admin) {
    std::lock_guard<std::mutex> lock(dataMutex);
    std::ifstream file(DATA_FILE);
    
    if (!file.is_open()) {
        std::cout << "No existing data file found. Starting fresh." << std::endl;
        return;
    }
    
    std::string line;
    // Skip header if exists
    std::getline(file, line);
    
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        
        auto fields = parseCSVLine(line);
        if (fields.size() < 7) continue;
        
        std::string id = fields[0];
        std::string name = fields[1];
        std::string email = fields[2];
        std::string phone = fields[3];
        std::string cardId = fields[4];
        int clearance = std::stoi(fields[5]);
        std::string type = fields[6];
        
        auto card = std::make_shared<Card>(cardId, intToClearanceLevel(clearance));
        
        if (type == "ADMIN" && fields.size() >= 8) {
            std::string password = fields[7];
            admin = std::make_shared<Admin>(id, name, email, phone, card, password);
        } else {
            users.push_back(std::make_shared<User>(id, name, email, phone, card));
        }
    }
    
    file.close();
    std::cout << "Loaded " << users.size() << " users and " 
              << (admin ? "1" : "0") << " admin from file." << std::endl;
}

void DataManager::saveToCSV(const std::vector<std::shared_ptr<User>>& users,
                             const std::shared_ptr<Admin>& admin) {
    std::lock_guard<std::mutex> lock(dataMutex);
    std::ofstream file(DATA_FILE);
    
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file for writing." << std::endl;
        return;
    }
    
    // Write header
    file << "ID,Name,Email,Phone,CardID,ClearanceLevel,Type,Password" << std::endl;
    
    // Write admin first
    if (admin) {
        file << admin->toCSV() << std::endl;
    }
    
    // Write all users
    for (const auto& user : users) {
        file << user->toCSV() << std::endl;
    }
    
    file.close();
    std::cout << "\n[SAVED] Data saved to " << DATA_FILE << " at " 
              << getCurrentTimestamp() << std::endl;
}

std::string DataManager::generateUniqueId(const std::string& baseName,
                                          const std::vector<std::shared_ptr<User>>& users) {
    // Create alphanumeric ID from name
    std::string baseId = baseName.substr(0, 3);
    std::transform(baseId.begin(), baseId.end(), baseId.begin(), ::toupper);
    
    // Check for duplicates and add suffix
    int suffix = 1;
    std::string newId = baseId;
    
    bool exists = true;
    while (exists) {
        exists = false;
        for (const auto& user : users) {
            if (user->getId() == newId) {
                exists = true;
                newId = baseId + std::to_string(suffix++);
                break;
            }
        }
    }
    
    return newId;
}

std::vector<std::string> DataManager::parseCSVLine(const std::string& line) {
    std::vector<std::string> fields;
    std::string field;
    bool inQuotes = false;
    
    for (char ch : line) {
        if (ch == '"') {
            inQuotes = !inQuotes;
        } else if (ch == ',' && !inQuotes) {
            fields.push_back(field);
            field.clear();
        } else {
            field += ch;
        }
    }
    fields.push_back(field);
    
    return fields;
}

std::string DataManager::escapeCSV(const std::string& field) {
    if (field.find(',') != std::string::npos || 
        field.find('"') != std::string::npos) {
        std::string escaped = "\"";
        for (char ch : field) {
            if (ch == '"') escaped += "\"\"";
            else escaped += ch;
        }
        escaped += "\"";
        return escaped;
    }
    return field;
}
