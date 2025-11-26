// ============================================================================
// FILE: Validator.h
// Description: Input validation functions
// ============================================================================

#ifndef VALIDATOR_H
#define VALIDATOR_H

#include "common.h"

class Validator {
public:
    // Validate password: min 8 chars, 1 upper, 1 lower, 1 number, 1 special
    static bool validatePassword(const std::string& password);

    // Validate email: local@domain.tld (at least one dot in domain)
    static bool validateEmail(const std::string& email);

    // Validate phone: 07XXXXXXXX or +467XXXXXXXX
    static bool validatePhone(const std::string& phone);

    // Generate a valid random email
    static std::string generateEmail(const std::string& name);

    // Generate a valid random phone
    static std::string generatePhone();
};

#endif // VALIDATOR_H