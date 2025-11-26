#ifndef SYSTEMMANAGER_H
#define SYSTEMMANAGER_H

#include "common.h"
#include "User.h"
#include "Admin.h"
#include "Floor.h"
#include "Cache.h"
#include "DataManager.h"

class SystemManager {
private:
    std::vector<std::shared_ptr<User>> users;
    std::shared_ptr<Admin> admin;
    std::vector<Floor> floors;
    LRUCache<std::string, std::shared_ptr<User>> userCache;
    DataManager dataManager;
    std::mutex systemMutex;
    std::atomic<bool> running;
    std::thread saveThread;

public:
    // Constructor
    SystemManager();

    // Destructor
    ~SystemManager();

    // Initialize system (load data, create floors)
    void initialize();

    // Start the system
    void run();

    // User operations
    void userMode();
    void userLogin();
    void userMenu(std::shared_ptr<User> user);
    void listFloorsForUser(std::shared_ptr<User> user);
    void accessFloor(std::shared_ptr<User> user);
    void showUserInfo(std::shared_ptr<User> user);
    void changeUserInfo(std::shared_ptr<User> user);

    // Admin operations
    void adminMode();
    void adminLogin();
    void adminMenu();
    void listFloorsForAdmin();
    void manageFloor();
    void listUsers();
    void manageUser();
    void createUser();
    void deleteUser();

    // Helper functions
    std::shared_ptr<User> findUser(const std::string& searchTerm);
    Floor* findFloor(const std::string& searchTerm);

    // Save system (thread-safe)
    void saveSystem();

    // Background thread for save command
    void saveThreadFunction();

    // Check for "scs -save" command
    void checkSaveCommand(const std::string& input);
};

#endif // SYSTEMMANAGER_H
