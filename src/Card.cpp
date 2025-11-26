// ============================================================================
// FILE: src/Card.cpp
// ============================================================================
#include "Card.h"

// Card implementation
Card::Card(const std::string& cardId, ClearanceLevel level)
    : id(cardId), clearanceLevel(level) {}

std::string Card::getId() const {
    return id;
}

ClearanceLevel Card::getClearanceLevel() const {
    return clearanceLevel;
}

int Card::getClearanceLevelInt() const {
    return clearanceLevelToInt(clearanceLevel);
}

void Card::setClearanceLevel(ClearanceLevel level) {
    clearanceLevel = level;
}