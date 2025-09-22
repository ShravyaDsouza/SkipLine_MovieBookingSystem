#ifndef BOOKING_MANAGER_H
#define BOOKING_MANAGER_H

#include "MovieBookingStructs.h"
#include "PromoManager.h"
#include "MovieManager.h" 

#include <vector>
#include <string>

class BookingManager {
public:
    BookingManager(PromoManager& promoMgr);

    Booking createBooking(long userId, long movieId, const std::string& showTime, const std::string& theater,
                          const std::vector<std::string>& seats, const std::string& promoCode, PaymentMethod method);
    bool cancelBooking(long bookingId);
    bool checkInBooking(long bookingId);

    std::vector<Booking> getUserBookings(long userId);
    std::vector<Booking> getBookingsByUser(long userId) const;
    Booking* getBookingById(long bookingId);

    bool modifyBooking(long bookingId, const std::string& newTiming, const std::string& newSeat);
    bool isModificationAllowed(const std::string& showTime) const;

    void loadBookingsFromFile();
    void saveBookingsToFile() const;

    void viewAllBookings();
    void viewBookingByID();
private:
    std::vector<Booking> bookings;
    PromoManager& promoManager;
    MovieManager& movieManager;

    float calculateTax(float baseAmount) const;
    float applyDiscount(float amount, const std::string& promoCode) const;
    float getServiceCharge(const std::string& theaterName) const;
};

#endif
