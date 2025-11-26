// ============================================================================
// FILE: Card.h
// Description: Card class - represents an access card with clearance level
// ============================================================================

#ifndef CARD_H
#define CARD_H

#include "common.h"

class Card {
private:
    std::string id;                    // Unique card ID
    ClearanceLevel clearanceLevel;     // Access clearance level (0-3)

public:
    // Constructor
    Card(const std::string& cardId, ClearanceLevel level);

    // Getters
    std::string getId() const;
    ClearanceLevel getClearanceLevel() const;
    int getClearanceLevelInt() const;

    // Setter
    void setClearanceLevel(ClearanceLevel level);
};

#endif // CARD_H