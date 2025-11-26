// ============================================================================
// FILE: src/DataGenerator.cpp
// Description: Generates 1000 users and 1 admin with random data
// ============================================================================

#include "common.h"
#include "User.h"
#include "Admin.h"
#include "DataManager.h"
#include "Validator.h"




void generateInitialData() {
    // Hardcoded name lists
    std::vector<std::string> firstNames = {
        "Carl", "Noah", "Oliver", "Elijah", "William", "James", "Benjamin", 
        "Lucas", "Henry", "Alexander", "Mason", "Michael", "Ethan", "Daniel", 
        "Jacob", "Logan", "Jackson", "Sebastian", "Jack", "Aiden", "Owen", 
        "Samuel", "Matthew", "Joseph", "David", "Wyatt", "Carter", "John", 
        "Jayden", "Gabriel", "Luke", "Anthony", "Isaac", "Grayson", "Julian", 
        "Ryan", "Levi", "Mateo", "Jaxon", "Ezra", "Aaron", "Charles", "Thomas", 
        "Hunter", "Caleb", "Josiah", "Christian", "Andrew", "Connor", "Jeremiah",
        "Olivia", "Emma", "Ava", "Sophia", "Isabella", "Charlotte", "Amelia", 
        "Mia", "Harper", "Evelyn", "Abigail", "Emily", "Elizabeth", "Avery", 
        "Sofia", "Ella", "Madison", "Scarlett", "Grace", "Chloe", "Victoria", 
        "Penelope", "Riley", "Aria", "Layla", "Lily", "Nora", "Zoe", "Hannah", 
        "Camila", "Stella", "Aurora", "Nova", "Willow", "Hazel", "Maya", 
        "Skylar", "Kinsley", "Naomi", "Elena", "Ruby", "Alice", "Claire", 
        "Lucy", "Violet", "Ivy", "Cora", "Audrey", "Anna", "Leah"
    };
    
    std::vector<std::string> lastNames = {
        "Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia", "Rodriguez", 
        "Martinez", "Hernandez", "Lopez", "Miller", "Wilson", "Moore", "Taylor", 
        "Anderson", "Thomas", "Jackson", "Martin", "Lee", "Perez", "White", 
        "Harris", "Clark", "Lewis", "Young", "Hall", "Walker", "Allen", 
        "Sanchez", "Kelly", "Baker", "King", "Wright", "Hill", "Scott", "Green", 
        "Adams", "Nelson", "Carter", "Mitchell", "Roberts", "Turner", "Phillips", 
        "Campbell", "Parker", "Evans", "Edwards", "Stewart", "Collins", "Davis"
    };
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> firstDist(0, firstNames.size() - 1);
    std::uniform_int_distribution<> lastDist(0, lastNames.size() - 1);
    std::uniform_int_distribution<> clearanceDist(0, 3);
    
    std::vector<std::shared_ptr<User>> users;
    DataManager dataManager;
    
    // Generate 1000 users
    std::cout << "Generating 1000 users..." << std::endl;
    for (int i = 0; i < 1000; ++i) {
        std::string firstName = firstNames[firstDist(gen)];
        std::string lastName = lastNames[lastDist(gen)];
        std::string fullName = firstName + " " + lastName;
        
        // Generate unique ID
        std::string userId = dataManager.generateUniqueId(firstName, users);
        
        // Generate email and phone
        std::string email = Validator::generateEmail(fullName);
        std::string phone = Validator::generatePhone();
        
        // Generate card with random clearance level
        int clearance = clearanceDist(gen);
        std::string cardId = "CARD" + std::to_string(i + 1);
        auto card = std::make_shared<Card>(cardId, intToClearanceLevel(clearance));
        
        // Create user
        auto user = std::make_shared<User>(userId, fullName, email, phone, card);
        users.push_back(user);
    }
    
    // Create admin with level 3 clearance
    std::cout << "Creating admin..." << std::endl;
    auto adminCard = std::make_shared<Card>("CARD_ADMIN", ClearanceLevel::LEVEL_3);
    auto admin = std::make_shared<Admin>("ADMIN", "Admin User", 
                                          "admin@company.com", "0712345678",
                                          adminCard, ADMIN_PASSWORD);
    
    // Save to file
    std::cout << "Saving to file..." << std::endl;
    dataManager.saveToCSV(users, admin);
    std::cout << "Data generation complete!" << std::endl;
}