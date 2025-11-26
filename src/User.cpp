// ============================================================================
// FILE: src/User.cpp
// ============================================================================

#include "User.h"

User::User(const std::string& userId, const std::string& userName,
           const std::string& userEmail, const std::string& userPhone,
           std::shared_ptr<Card> userCard)
    : id(userId), name(userName), email(userEmail), 
      phone(userPhone), card(userCard) {}

std::string User::getId() const { return id; }
std::string User::getName() const { return name; }
std::string User::getEmail() const { return email; }
std::string User::getPhone() const { return phone; }
std::shared_ptr<Card> User::getCard() const { return card; }

void User::setName(const std::string& newName) { name = newName; }
void User::setEmail(const std::string& newEmail) { email = newEmail; }
void User::setPhone(const std::string& newPhone) { phone = newPhone; }

void User::displayInfo() const {
    std::cout << "\n=== Employee Information ===" << std::endl;
    std::cout << "ID: " << id << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Email: " << email << std::endl;
    std::cout << "Phone: " << phone << std::endl;
    std::cout << "Card ID: " << card->getId() << std::endl;
    std::cout << "Clearance Level: " << card->getClearanceLevelInt() << std::endl;
}

std::string User::toCSV() const {
    return id + "," + name + "," + email + "," + phone + "," +
           card->getId() + "," + std::to_string(card->getClearanceLevelInt()) + ",USER";
}
