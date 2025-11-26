// ============================================================================
// FILE: src/SystemManager.cpp (Part 1 - Constructor and Initialization)
// ============================================================================

#include "SystemManager.h"
#include "DataGenerator.h"
#include "Validator.h"

SystemManager::SystemManager() 
    : userCache(CACHE_SIZE), running(true) {
    // Initialize default floors
    floors.push_back(Floor("F1", "Ground Floor", ClearanceLevel::LEVEL_0));
    floors.push_back(Floor("F2", "Office Floor", ClearanceLevel::LEVEL_1));
    floors.push_back(Floor("F3", "Server Room", ClearanceLevel::LEVEL_2));
    floors.push_back(Floor("F4", "Executive Suite", ClearanceLevel::LEVEL_3));
}

SystemManager::~SystemManager() {
    running = false;
    if (saveThread.joinable()) {
        saveThread.join();
    }
}

void SystemManager::initialize() {
    // Create data directory if it doesn't exist
    #ifdef _WIN32
        system("if not exist data mkdir data");
    #else
        system("mkdir -p data");
    #endif
    
    // Check if data file exists
    std::ifstream checkFile(DATA_FILE);
    if (!checkFile.good()) {
        std::cout << "No data file found. Generating initial data..." << std::endl;
        generateInitialData();
    }
    checkFile.close();
    
    // Load data from CSV
    dataManager.loadFromCSV(users, admin);
    
    // Start background save thread
    saveThread = std::thread(&SystemManager::saveThreadFunction, this);
}

void SystemManager::run() {
    std::cout << "\n╔════════════════════════════════════════╗" << std::endl;
    std::cout << "║   SECURE CARD ACCESS SYSTEM           ║" << std::endl;
    std::cout << "╚════════════════════════════════════════╝" << std::endl;
    
    while (running) {
        std::cout << "\n=== Main Menu ===" << std::endl;
        std::cout << "1. User Login" << std::endl;
        std::cout << "2. Admin Login" << std::endl;
        std::cout << "3. Exit" << std::endl;
        std::cout << "Choice: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        checkSaveCommand(choice);
        
        if (choice == "1") {
            userMode();
        } else if (choice == "2") {
            adminMode();
        } else if (choice == "3") {
            std::cout << "Saving data and exiting..." << std::endl;
            saveSystem();
            running = false;
        } else {
            std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
}

// Background thread function to handle save command
void SystemManager::saveThreadFunction() {
    // This thread runs alongside main thread, scheduled by OS
    // It waits for save operations to be triggered
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void SystemManager::checkSaveCommand(const std::string& input) {
    if (input == "scs -save") {
        // Trigger save in separate thread context
        std::thread([this]() {
            saveSystem();
        }).detach();
    }
}

void SystemManager::saveSystem() {
    std::lock_guard<std::mutex> lock(systemMutex);
    dataManager.saveToCSV(users, admin);
}

// ============================================================================
// FILE: src/SystemManager.cpp (Part 2 - User Operations)
// ============================================================================

void SystemManager::userMode() {
    userLogin();
}

void SystemManager::userLogin() {
    std::cout << "\n=== User Login ===" << std::endl;
    std::cout << "Enter Employee ID or Name: ";
    std::string searchTerm;
    std::getline(std::cin, searchTerm);
    checkSaveCommand(searchTerm);
    
    auto user = findUser(searchTerm);
    if (user) {
        std::cout << "Login successful! Welcome, " << user->getName() << std::endl;
        userMenu(user);
    } else {
        std::cout << "User not found." << std::endl;
    }
}

void SystemManager::userMenu(std::shared_ptr<User> user) {
    while (true) {
        std::cout << "\n=== User Menu ===" << std::endl;
        std::cout << "1. List all available floors" << std::endl;
        std::cout << "2. Show personal information" << std::endl;
        std::cout << "3. Log out" << std::endl;
        std::cout << "Choice: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        checkSaveCommand(choice);
        
        if (choice == "1") {
            listFloorsForUser(user);
        } else if (choice == "2") {
            showUserInfo(user);
        } else if (choice == "3") {
            std::cout << "Logging out..." << std::endl;
            return;
        } else {
            std::cout << "Invalid choice." << std::endl;
        }
    }
}

void SystemManager::listFloorsForUser(std::shared_ptr<User> user) {
    std::cout << "\n=== Available Floors ===" << std::endl;
    for (size_t i = 0; i < floors.size(); ++i) {
        std::cout << (i + 1) << ". " << floors[i].getName() 
                  << " (Required Clearance: " 
                  << clearanceLevelToInt(floors[i].getRequiredClearance()) << ")" 
                  << std::endl;
    }
    
    std::cout << "\nOptions:" << std::endl;
    std::cout << "1. Access a floor" << std::endl;
    std::cout << "2. Back" << std::endl;
    std::cout << "Choice: ";
    
    std::string choice;
    std::getline(std::cin, choice);
    checkSaveCommand(choice);
    
    if (choice == "1") {
        accessFloor(user);
    }
}

void SystemManager::accessFloor(std::shared_ptr<User> user) {
    std::cout << "Enter floor number (1-" << floors.size() << "): ";
    std::string floorNum;
    std::getline(std::cin, floorNum);
    checkSaveCommand(floorNum);
    
    try {
        int num = std::stoi(floorNum);
        if (num < 1 || num > static_cast<int>(floors.size())) {
            std::cout << "Invalid floor number." << std::endl;
            return;
        }
        
        Floor& floor = floors[num - 1];
        bool authorized = floor.attemptAccess(*user);
        
        // Log and print result
        std::string timestamp = getCurrentTimestamp();
        std::cout << "\n=== Access Attempt ===" << std::endl;
        std::cout << "Floor: " << floor.getName() << std::endl;
        std::cout << "Employee: " << user->getName() << " (" << user->getId() << ")" << std::endl;
        std::cout << "Time: " << timestamp << std::endl;
        std::cout << "Result: " << (authorized ? "ACCESS GRANTED" : "ACCESS DENIED") << std::endl;
        
        if (!authorized) {
            std::cout << "Reason: Insufficient clearance level. Required: " 
                      << clearanceLevelToInt(floor.getRequiredClearance())
                      << ", Your level: " << user->getCard()->getClearanceLevelInt() << std::endl;
        }
    } catch (const std::exception& e) {
        std::cout << "Invalid input." << std::endl;
    }
}

void SystemManager::showUserInfo(std::shared_ptr<User> user) {
    user->displayInfo();
    
    std::cout << "\nOptions:" << std::endl;
    std::cout << "1. Change information" << std::endl;
    std::cout << "2. Back" << std::endl;
    std::cout << "Choice: ";
    
    std::string choice;
    std::getline(std::cin, choice);
    checkSaveCommand(choice);
    
    if (choice == "1") {
        changeUserInfo(user);
    }
}

void SystemManager::changeUserInfo(std::shared_ptr<User> user) {
    std::cout << "\n=== Change Information ===" << std::endl;
    std::cout << "1. Change name" << std::endl;
    std::cout << "2. Change email" << std::endl;
    std::cout << "3. Change phone" << std::endl;
    std::cout << "4. Back" << std::endl;
    std::cout << "Choice: ";
    
    std::string choice;
    std::getline(std::cin, choice);
    checkSaveCommand(choice);
    
    try {
        if (choice == "1") {
            std::cout << "Enter new name: ";
            std::string newName;
            std::getline(std::cin, newName);
            checkSaveCommand(newName);
            user->setName(newName);
            std::cout << "Name updated successfully." << std::endl;
        } else if (choice == "2") {
            std::cout << "Enter new email: ";
            std::string newEmail;
            std::getline(std::cin, newEmail);
            checkSaveCommand(newEmail);
            
            if (Validator::validateEmail(newEmail)) {
                user->setEmail(newEmail);
                std::cout << "Email updated successfully." << std::endl;
            } else {
                std::cout << "Invalid email format. Email not updated." << std::endl;
            }
        } else if (choice == "3") {
            std::cout << "Enter new phone (07XXXXXXXX or +467XXXXXXXX): ";
            std::string newPhone;
            std::getline(std::cin, newPhone);
            checkSaveCommand(newPhone);
            
            if (Validator::validatePhone(newPhone)) {
                user->setPhone(newPhone);
                std::cout << "Phone updated successfully." << std::endl;
            } else {
                std::cout << "Invalid phone format. Phone not updated." << std::endl;
            }
        }
    } catch (const std::exception& e) {
        std::cout << "Error updating information: " << e.what() << std::endl;
    }
}

// ============================================================================
// FILE: src/SystemManager.cpp (Part 3 - Admin Operations)
// ============================================================================

void SystemManager::adminMode() {
    adminLogin();
}

void SystemManager::adminLogin() {
    if (!admin) {
        std::cout << "No admin account exists in the system." << std::endl;
        return;
    }
    
    std::cout << "\n=== Admin Login ===" << std::endl;
    std::cout << "Enter Admin ID: ";
    std::string adminId;
    std::getline(std::cin, adminId);
    checkSaveCommand(adminId);
    
    if (adminId != admin->getId()) {
        std::cout << "Invalid admin ID." << std::endl;
        return;
    }
    
    std::cout << "Enter Password: ";
    std::string password;
    std::getline(std::cin, password);
    checkSaveCommand(password);
    
    if (admin->verifyPassword(password)) {
        std::cout << "Admin login successful!" << std::endl;
        adminMenu();
    } else {
        std::cout << "Incorrect password." << std::endl;
    }
}

void SystemManager::adminMenu() {
    while (true) {
        std::cout << "\n=== Admin Menu ===" << std::endl;
        std::cout << "1. List all floors" << std::endl;
        std::cout << "2. List all users" << std::endl;
        std::cout << "3. Create new user" << std::endl;
        std::cout << "4. Log out" << std::endl;
        std::cout << "Choice: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        checkSaveCommand(choice);
        
        if (choice == "1") {
            listFloorsForAdmin();
        } else if (choice == "2") {
            listUsers();
        } else if (choice == "3") {
            createUser();
        } else if (choice == "4") {
            std::cout << "Logging out..." << std::endl;
            return;
        } else {
            std::cout << "Invalid choice." << std::endl;
        }
    }
}

void SystemManager::listFloorsForAdmin() {
    std::cout << "\n=== All Floors ===" << std::endl;
    for (size_t i = 0; i < floors.size(); ++i) {
        std::cout << (i + 1) << ". " << floors[i].getName() 
                  << " (Clearance: " 
                  << clearanceLevelToInt(floors[i].getRequiredClearance()) << ")" 
                  << std::endl;
    }
    
    std::cout << "\nEnter floor number to manage (or 0 to go back): ";
    std::string floorNum;
    std::getline(std::cin, floorNum);
    checkSaveCommand(floorNum);
    
    try {
        int num = std::stoi(floorNum);
        if (num == 0) return;
        if (num < 1 || num > static_cast<int>(floors.size())) {
            std::cout << "Invalid floor number." << std::endl;
            return;
        }
        
        manageFloor();
    } catch (const std::exception& e) {
        std::cout << "Invalid input." << std::endl;
    }
}

void SystemManager::manageFloor() {
    std::cout << "Enter floor name or number: ";
    std::string searchTerm;
    std::getline(std::cin, searchTerm);
    checkSaveCommand(searchTerm);
    
    Floor* floor = findFloor(searchTerm);
    if (!floor) {
        std::cout << "Floor not found." << std::endl;
        return;
    }
    
    while (true) {
        std::cout << "\n=== Manage Floor: " << floor->getName() << " ===" << std::endl;
        std::cout << "1. View access history" << std::endl;
        std::cout << "2. Change floor name" << std::endl;
        std::cout << "3. Change clearance level" << std::endl;
        std::cout << "4. Back" << std::endl;
        std::cout << "Choice: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        checkSaveCommand(choice);
        
        if (choice == "1") {
            floor->displayAccessHistory();
        } else if (choice == "2") {
            std::cout << "Enter new floor name: ";
            std::string newName;
            std::getline(std::cin, newName);
            checkSaveCommand(newName);
            floor->setName(newName);
            std::cout << "Floor name updated." << std::endl;
        } else if (choice == "3") {
            std::cout << "Enter new clearance level (0-3): ";
            std::string levelStr;
            std::getline(std::cin, levelStr);
            checkSaveCommand(levelStr);
            
            try {
                int level = std::stoi(levelStr);
                if (level >= 0 && level <= 3) {
                    floor->setRequiredClearance(intToClearanceLevel(level));
                    std::cout << "Clearance level updated." << std::endl;
                } else {
                    std::cout << "Invalid clearance level." << std::endl;
                }
            } catch (const std::exception& e) {
                std::cout << "Invalid input." << std::endl;
            }
        } else if (choice == "4") {
            return;
        }
    }
}

void SystemManager::listUsers() {
    std::cout << "\n=== All Users ===" << std::endl;
    std::cout << std::left << std::setw(10) << "ID" 
              << std::setw(25) << "Name" 
              << std::setw(30) << "Email" 
              << "Clearance" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    
    for (const auto& user : users) {
        std::cout << std::left << std::setw(10) << user->getId()
                  << std::setw(25) << user->getName()
                  << std::setw(30) << user->getEmail()
                  << user->getCard()->getClearanceLevelInt() << std::endl;
    }
    
    std::cout << "\nEnter user ID or name to manage (or 'back' to return): ";
    std::string searchTerm;
    std::getline(std::cin, searchTerm);
    checkSaveCommand(searchTerm);
    
    if (searchTerm == "back") return;
    
    auto user = findUser(searchTerm);
    if (user) {
        manageUser();
    } else {
        std::cout << "User not found." << std::endl;
    }
}

void SystemManager::manageUser() {
    std::cout << "Enter user ID or name: ";
    std::string searchTerm;
    std::getline(std::cin, searchTerm);
    checkSaveCommand(searchTerm);
    
    auto user = findUser(searchTerm);
    if (!user) {
        std::cout << "User not found." << std::endl;
        return;
    }
    
    while (true) {
        std::cout << "\n=== Manage User: " << user->getName() << " ===" << std::endl;
        std::cout << "1. Change name" << std::endl;
        std::cout << "2. Change email" << std::endl;
        std::cout << "3. Change phone" << std::endl;
        std::cout << "4. Delete user" << std::endl;
        std::cout << "5. Back" << std::endl;
        std::cout << "Choice: ";
        
        std::string choice;
        std::getline(std::cin, choice);
        checkSaveCommand(choice);
        
        if (choice == "1") {
            std::cout << "Enter new name: ";
            std::string newName;
            std::getline(std::cin, newName);
            checkSaveCommand(newName);
            user->setName(newName);
            std::cout << "Name updated." << std::endl;
        } else if (choice == "2") {
            std::cout << "Enter new email: ";
            std::string newEmail;
            std::getline(std::cin, newEmail);
            checkSaveCommand(newEmail);
            
            if (Validator::validateEmail(newEmail)) {
                user->setEmail(newEmail);
                std::cout << "Email updated." << std::endl;
            } else {
                std::cout << "Invalid email format." << std::endl;
            }
        } else if (choice == "3") {
            std::cout << "Enter new phone: ";
            std::string newPhone;
            std::getline(std::cin, newPhone);
            checkSaveCommand(newPhone);
            
            if (Validator::validatePhone(newPhone)) {
                user->setPhone(newPhone);
                std::cout << "Phone updated." << std::endl;
            } else {
                std::cout << "Invalid phone format." << std::endl;
            }
        } else if (choice == "4") {
            deleteUser();
            return;
        } else if (choice == "5") {
            return;
        }
    }
}

void SystemManager::createUser() {
    std::lock_guard<std::mutex> lock(systemMutex);
    
    std::cout << "\n=== Create New User ===" << std::endl;
    
    std::cout << "Enter name: ";
    std::string name;
    std::getline(std::cin, name);
    checkSaveCommand(name);
    
    std::cout << "Enter email: ";
    std::string email;
    std::getline(std::cin, email);
    checkSaveCommand(email);
    
    if (!Validator::validateEmail(email)) {
        std::cout << "Invalid email format. User not created." << std::endl;
        return;
    }
    
    std::cout << "Enter phone (07XXXXXXXX or +467XXXXXXXX): ";
    std::string phone;
    std::getline(std::cin, phone);
    checkSaveCommand(phone);
    
    if (!Validator::validatePhone(phone)) {
        std::cout << "Invalid phone format. User not created." << std::endl;
        return;
    }
    
    std::cout << "Enter clearance level (0-3): ";
    std::string levelStr;
    std::getline(std::cin, levelStr);
    checkSaveCommand(levelStr);
    
    try {
        int level = std::stoi(levelStr);
        if (level < 0 || level > 3) {
            std::cout << "Invalid clearance level. User not created." << std::endl;
            return;
        }
        
        // Generate unique ID
        std::string userId = dataManager.generateUniqueId(name, users);
        
        // Create card
        std::string cardId = "CARD" + std::to_string(users.size() + 1);
        auto card = std::make_shared<Card>(cardId, intToClearanceLevel(level));
        
        // Create user
        auto newUser = std::make_shared<User>(userId, name, email, phone, card);
        users.push_back(newUser);
        
        std::cout << "User created successfully with ID: " << userId << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error creating user: " << e.what() << std::endl;
    }
}

void SystemManager::deleteUser() {
    std::lock_guard<std::mutex> lock(systemMutex);
    
    std::cout << "Enter user ID to delete: ";
    std::string userId;
    std::getline(std::cin, userId);
    checkSaveCommand(userId);
    
    auto it = std::find_if(users.begin(), users.end(),
        [&userId](const std::shared_ptr<User>& u) { return u->getId() == userId; });
    
    if (it != users.end()) {
        std::cout << "Are you sure you want to delete user " 
                  << (*it)->getName() << "? (yes/no): ";
        std::string confirm;
        std::getline(std::cin, confirm);
        checkSaveCommand(confirm);
        
        if (confirm == "yes") {
            users.erase(it);
            std::cout << "User and their card deleted successfully." << std::endl;
        } else {
            std::cout << "Deletion cancelled." << std::endl;
        }
    } else {
        std::cout << "User not found." << std::endl;
    }
}

// ============================================================================
// FILE: src/SystemManager.cpp (Part 4 - Helper Functions)
// ============================================================================

std::shared_ptr<User> SystemManager::findUser(const std::string& searchTerm) {
    // Check cache first
    auto cachedUser = userCache.get(searchTerm);
    if (cachedUser) {
        return *cachedUser;
    }
    
    // Search in users list
    for (const auto& user : users) {
        if (user->getId() == searchTerm || user->getName() == searchTerm) {
            userCache.put(searchTerm, user);  // Add to cache
            return user;
        }
    }
    
    return nullptr;
}

Floor* SystemManager::findFloor(const std::string& searchTerm) {
    // Try to find by name first
    for (auto& floor : floors) {
        if (floor.getName() == searchTerm) {
            return &floor;
        }
    }
    
    // Try to find by number
    try {
        int num = std::stoi(searchTerm);
        if (num >= 1 && num <= static_cast<int>(floors.size())) {
            return &floors[num - 1];
        }
    } catch (...) {
        // Not a number, continue
    }
    
    return nullptr;
}