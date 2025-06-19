#ifndef BOOKING_MANAGER_H
#define BOOKING_MANAGER_H

#include "MovieBookingStructs.h"
#include <vector>
#include <string>

class BookingManager {
public:
    BookingManager();

    Booking createBooking(long userId, long movieId, const std::string& showTime, const std::string& theater, const std::vector<std::string>& seats, const std::string& promoCode, PaymentMethod method);
    bool cancelBooking(long bookingId);
    bool checkInBooking(long bookingId);
    std::vector<Booking> getUserBookings(long userId);
    Booking* getBookingById(long bookingId);

    void loadBookingsFromFile();
    void saveBookingsToFile();

private:
    std::vector<Booking> bookings;
    float calculateTax(float baseAmount);
    float applyDiscount(float amount, const std::string& promoCode);
    float getServiceCharge(const std::string& theaterName);
};

#endif
