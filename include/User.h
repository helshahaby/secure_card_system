// ============================================================================
// FILE: User.h
// Description: User class - represents an employee with access card
// ============================================================================

#ifndef USER_H
#define USER_H

#include "common.h"
#include "Card.h"

class User {
protected:
    std::string id;           // Unique employee ID
    std::string name;         // Full name
    std::string email;        // Email address
    std::string phone;        // Phone number
    std::shared_ptr<Card> card;  // Associated access card (not null)

public:
    // Constructor
    User(const std::string& userId, const std::string& userName,
         const std::string& userEmail, const std::string& userPhone,
         std::shared_ptr<Card> userCard);

    // Virtual destructor for inheritance
    virtual ~User() = default;

    // Getters
    std::string getId() const;
    std::string getName() const;
    std::string getEmail() const;
    std::string getPhone() const;
    std::shared_ptr<Card> getCard() const;

    // Setters (only name, email, phone can be changed by user)
    void setName(const std::string& newName);
    void setEmail(const std::string& newEmail);
    void setPhone(const std::string& newPhone);

    // Display user information
    void displayInfo() const;

    // Convert to CSV format
    virtual std::string toCSV() const;
};

#endif // USER_H