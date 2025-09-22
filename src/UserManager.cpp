#include "UserManager.h"
#include "FileIO.h"
#include "Hasher.h"
#include <iostream>

UserManager::UserManager() {
    users = FileIO::loadUsers("data/users.txt");
}

bool UserManager::isEmailRegistered(const std::string& email) {
    for (const auto& user : users) {
        if (user.email == email) return true;
    }
    return false;
}

bool UserManager::registerUser(const User& newUser) {
    if (isEmailRegistered(newUser.email)) {
        std::cout << "Email already registered.\n";
        return false;
    }

    users.push_back(newUser);
    FileIO::saveUsers("data/users.txt", users);
    return true;
}

User* UserManager::loginUser(const std::string& email, const std::string& password) {
    std::string hashedInput = hashPassword(password);

    for (auto& user : users) {
        if (user.email == email && user.hashPassword == hashedInput) {
            return &user;
        }
    }
    return nullptr;
}

User* UserManager::getUserById(long userId) {
    for (auto& user : users) {
        if (user.u_id == userId) return &user;
    }
    return nullptr;
}

std::vector<User> UserManager::getAllUsers() const {
    return users;
}

void UserManager::deleteUser(long userId) {
    auto it = std::remove_if(users.begin(), users.end(), [userId](const User& u) {
        return u.u_id == userId;
    });

    if (it != users.end()) {
        users.erase(it, users.end());
        saveUsersToFile(); 
        std::cout << "User with ID " << userId << " deleted.\n";
    } else {
        std::cout << "User with ID " << userId << " not found.\n";
    }
}