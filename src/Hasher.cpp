#include "Hasher.h"
#include <functional>
#include <sstream>

std::string hashPassword(const std::string& password) {
    std::hash<std::string> hasher;
    std::size_t hashed = hasher(password);

    std::ostringstream oss;
    oss << hashed;
    return oss.str();  
}
