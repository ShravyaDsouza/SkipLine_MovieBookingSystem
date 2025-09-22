#include "Validator.h"
#include <regex>

bool isValidEmail(const std::string& email) {
    std::regex pattern(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    return std::regex_match(email, pattern);
}

bool isValidPhone(const std::string& phone) {
    if (phone.length() != 10) return false;
    return phone[0] != '0' && std::all_of(phone.begin(), phone.end(), ::isdigit);
}
