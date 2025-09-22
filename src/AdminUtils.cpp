#include "AdminUtils.h"
#include <fstream>
#include <iostream>

std::pair<std::string, std::string> loadAdminCredentials(const std::string& filePath) {
    std::ifstream file(filePath);
    std::string email, hashedPassword;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open admin credentials file.\n";
        return {"", ""};
    }

    std::getline(file, email);
    std::getline(file, hashedPassword);
    file.close();

    return {email, hashedPassword};
}
