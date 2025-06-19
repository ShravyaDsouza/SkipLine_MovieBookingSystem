#include "headers/FileIO.h"
#include <fstream>
#include <sstream>

// --- USERS ---

std::vector<User> FileIO::loadUsers(const std::string& filename) {
    std::vector<User> users;
    std::ifstream infile(filename);
    std::string line;

    while (getline(infile, line)) {
        users.push_back(deserializeUser(line));
    }

    infile.close();
    return users;
}

void FileIO::saveUsers(const std::string& filename, const std::vector<User>& users) {
    std::ofstream outfile(filename);
    for (const auto& user : users) {
        outfile << serializeUser(user) << "\n";
    }
    outfile.close();
}

// --- Internal Helper ---

std::string FileIO::serializeUser(const User& user) {
    std::ostringstream oss;
    oss << user.u_id << ","
        << user.type << ","
        << user.phoneno << ","
        << user.name << ","
        << user.email << ","
        << user.hashPassword << ","
        << user.isAdmin;

    // bookingHistory can be stored later if needed
    return oss.str();
}

User FileIO::deserializeUser(const std::string& line) {
    std::istringstream iss(line);
    std::string token;
    User user;

    getline(iss, token, ','); user.u_id = stol(token);
    getline(iss, user.type, ',');
    getline(iss, user.phoneno, ',');
    getline(iss, user.name, ',');
    getline(iss, user.email, ',');
    getline(iss, user.hashPassword, ',');
    getline(iss, token, ','); user.isAdmin = (token == "1");

    return user;
}
