#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include "MovieBookingStructs.h"
#include <vector>
#include <string>

class UserManager {
public:
    UserManager();

    bool registerUser(const User& newUser);
    User* loginUser(const std::string& email, const std::string& password);
    bool isEmailRegistered(const std::string& email);
    User* getUserById(long userId);
    std::vector<User> getAllUsers() const;

private:
    std::vector<User> users;
    void loadUsersFromFile();
    void saveUsersToFile();
};

#endif
