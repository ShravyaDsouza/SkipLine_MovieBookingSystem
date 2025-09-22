#ifndef ADMIN_MANAGER_H
#define ADMIN_MANAGER_H

#include "MovieBookingStructs.h"
#include "UserManager.h"
#include "MovieManager.h"
#include "TheaterManager.h"
#include "BookingManager.h"
#include "AnalyticsManager.h"
#include "PromoManager.h"
#include <string>

class AdminManager {
public:
    AdminManager() = default;

    // -- Movie Manager --
    void addMovie(MovieManager& movieManager,const TheaterManager& theaterManager);
    void updateMovie(MovieManager& movieManager, long movieId);
    void deleteMovie(MovieManager& movieManager, long movieId);

    // -- Theater Manager --
    void addTheater(TheaterManager& theaterManager);
    void updateTheaterDetails(TheaterManager& theaterManager);
    void deleteTheater(TheaterManager& theaterManager);
    
    // -- User Manager --
    void viewAllUsers(const UserManager& userManager) const;
    void deleteUser(UserManager& userManager, long userId);
    void viewUserDetails(UserManager& userManager, long userId) const;


    void viewAllBookings(const BookingManager& bookingManager) const;
   
    // -- Analytics Manager -- 
    void viewAnalytics(const AnalyticsManager& analyticsManager) const;
    void resetAnalytics(AnalyticsManager& analyticsManager);

    // -- Promo Manager --
    void createPromoCode(PromoManager& promoManager);
    void viewAllPromos(const PromoManager& promoManager) const;
    void removePromo(PromoManager& promoManager);
};

#endif
