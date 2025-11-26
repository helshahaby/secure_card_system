#include <iostream>
#include "SystemManager.h"
#include "Validator.h"

int main() {
    try {
        // Create and initialize system manager
        SystemManager system;
        system.initialize();
        
        // Run the system
        system.run();
        
        std::cout << "\nThank you for using the Secure Card System!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
}