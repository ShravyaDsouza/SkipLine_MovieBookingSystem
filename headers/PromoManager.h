#ifndef PROMO_MANAGER_H
#define PROMO_MANAGER_H

#include "MovieBookingStructs.h"
#include <vector>
#include <string>
#include <map>

class PromoManager {
public:
    PromoManager();

    bool isValidPromo(const std::string& code) const;
    float getDiscountPercent(const std::string& code) const;

    void addPromo(const PromoCode& promo);
    void viewPromo() const;
    void removePromo(const std::string& code);

    void loadPromosFromFile();
    void savePromosToFile() const;

private:
    std::vector<PromoCode> promoCodes;

    PromoCode* findPromoByCode(const std::string& code);
    const PromoCode* findPromoByCode(const std::string& code) const;
};

#endif
