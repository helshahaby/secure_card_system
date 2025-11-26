// ============================================================================
// FILE: src/Validator.cpp
// ============================================================================
#include "Validator.h"

bool Validator::validatePassword(const std::string& password) {
    if (password.length() < 8) return false;
    
    bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
    
    for (char ch : password) {
        if (std::isupper(ch)) hasUpper = true;
        else if (std::islower(ch)) hasLower = true;
        else if (std::isdigit(ch)) hasDigit = true;
        else if (std::ispunct(ch)) hasSpecial = true;
    }
    
    return hasUpper && hasLower && hasDigit && hasSpecial;
}

bool Validator::validateEmail(const std::string& email) {
    // Regex for local@domain.tld (at least one dot in domain)
    std::regex emailPattern(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return std::regex_match(email, emailPattern);
}

bool Validator::validatePhone(const std::string& phone) {
    // 07XXXXXXXX or +467XXXXXXXX
    std::regex phonePattern(R"(^(07\d{8}|\+467\d{8})$)");
    return std::regex_match(phone, phonePattern);
}

std::string Validator::generateEmail(const std::string& name) {
    std::string emailName = name;
    std::transform(emailName.begin(), emailName.end(), emailName.begin(), ::tolower);
    std::replace(emailName.begin(), emailName.end(), ' ', '.');
    return emailName + "@company.com";
}

std::string Validator::generatePhone() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(10000000, 99999999);
    return "07" + std::to_string(dis(gen));
}
