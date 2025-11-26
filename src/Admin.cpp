// ============================================================================
// FILE: src/Admin.cpp
// ============================================================================
#include "Admin.h"

Admin::Admin(const std::string& adminId, const std::string& adminName,
             const std::string& adminEmail, const std::string& adminPhone,
             std::shared_ptr<Card> adminCard, const std::string& adminPassword)
    : User(adminId, adminName, adminEmail, adminPhone, adminCard),
      password(adminPassword) {}

bool Admin::verifyPassword(const std::string& inputPassword) const {
    return password == inputPassword;
}

std::string Admin::toCSV() const {
    return id + "," + name + "," + email + "," + phone + "," +
           card->getId() + "," + std::to_string(card->getClearanceLevelInt()) + 
           ",ADMIN," + password;
}