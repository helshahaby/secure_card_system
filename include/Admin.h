// ============================================================================
// FILE: Admin.h
// Description: Admin class - inherits from User, has password
// ============================================================================

#ifndef ADMIN_H
#define ADMIN_H

#include "User.h"
#include <memory>

class Admin : public User {
private:
    std::string password;  // Admin password (validated)

public:
    // Constructor
    Admin(const std::string& adminId, const std::string& adminName,
          const std::string& adminEmail, const std::string& adminPhone,
          std::shared_ptr<Card> adminCard, const std::string& adminPassword);

    // Password verification
    bool verifyPassword(const std::string& inputPassword) const;

    // Override toCSV to include password
    std::string toCSV() const override;
};

#endif // ADMIN_H