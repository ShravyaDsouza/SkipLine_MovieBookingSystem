#ifndef ADMIN_UTILS_H
#define ADMIN_UTILS_H

#include <string>
#include <utility>

std::pair<std::string, std::string> loadAdminCredentials(const std::string& filePath = "admin.txt");

#endif
