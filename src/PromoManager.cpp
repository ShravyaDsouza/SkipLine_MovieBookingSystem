#include "PromoManager.h"
#include "FileIO.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <algorithm>

PromoManager::PromoManager() {
    loadPromosFromFile();
}

void PromoManager::addPromo(const PromoCode& promo) {
    promoCodes.push_back(promo);
    savePromosToFile();
    std::cout << "Promo code added successfully.\n";
}

bool PromoManager::isValidPromo(const std::string& code) const {
    const PromoCode* promo = findPromoByCode(code);
    if (promo && promo->isValid) {
        time_t now = time(nullptr);
        return now < promo->expiryDate;
    }
    return false;
}

float PromoManager::getDiscountPercent(const std::string& code) const {
    const PromoCode* promo = findPromoByCode(code);
    return (promo && isValidPromo(code)) ? promo->discountPercent : 0.0f;
}

void PromoManager::viewPromo() const {
    std::cout << "\n--- Available Promo Codes ---\n";
    for (const auto& promo : promoCodes) {
        std::cout << "Code: " << promo.code
                  << " | Discount: " << promo.discountPercent << "%"
                  << " | Created: " << std::put_time(std::localtime(&promo.createdWhen), "%Y-%m-%d")
                  << " | Expires: " << std::put_time(std::localtime(&promo.expiryDate), "%Y-%m-%d")
                  << " | Usage Limit: " << promo.usageLimit
                  << " | Min Purchase: ₹" << promo.minPurchaseAmount
                  << " | Valid: " << (promo.isValid ? "Yes" : "No") << "\n";
    }
}

void PromoManager::removePromo(const std::string& code) {
    for (auto& promo : promoCodes) {
        if (promo.code == code) {
            promo.isValid = false;
            std::cout << "Promo code marked as expired.\n";
            savePromosToFile();
            return;
        }
    }
    std::cout << "Promo code not found.\n";
}

void PromoManager::loadPromosFromFile() {
    promoCodes = FileIO::loadPromos("data/promos.txt");
}

void PromoManager::savePromosToFile() const {
    FileIO::savePromos("data/promos.txt", promoCodes);
}

PromoCode* PromoManager::findPromoByCode(const std::string& code) {
    for (auto& promo : promoCodes) {
        if (promo.code == code) return &promo;
    }
    return nullptr;
}

const PromoCode* PromoManager::findPromoByCode(const std::string& code) const {
    for (const auto& promo : promoCodes) {
        if (promo.code == code) return &promo;
    }
    return nullptr;
}
