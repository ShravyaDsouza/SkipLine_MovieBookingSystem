#ifndef PROMO_MANAGER_H
#define PROMO_MANAGER_H

#include "MovieBookingStructs.h"
#include <vector>
#include <string>

class PromoManager {
public:
    PromoManager();

    bool isValidPromo(const std::string& code) const;
    float getDiscountPercent(const std::string& code) const;
    void addPromo(const PromoCode& promo);

    void loadPromosFromFile();
    void savePromosToFile();

private:
    std::vector<PromoCode> promoCodes;
};

#endif
