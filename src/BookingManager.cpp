#include "BookingManager.h"
#include "FileIO.h"
#include "PromoManager.h"
#include <iostream>
#include <ctime>
#include <sstream>
#include <iomanip>

BookingManager::BookingManager(PromoManager& promoMgr, MovieManager& movieMgr)
    : promoManager(promoMgr), movieManager(movieMgr) {
    loadBookingsFromFile();
}



Booking BookingManager::createBooking(long userId, long movieId, const std::string& showTime,
                                      const std::string& theater, const std::vector<std::string>& seats,
                                      const std::string& promoCode, PaymentMethod method) {
    Booking newBooking;
    newBooking.b_id = generateUniqueBookingId();
    newBooking.u_id = userId;
    newBooking.m_id = movieId;
    newBooking.showTime = showTime;
    newBooking.theater = theater;
    newBooking.seats = seats;
    newBooking.paymentMethod = method;
    newBooking.bookingTime = std::time(nullptr);
    newBooking.bookingDate = getCurrentDate();
    newBooking.promoCode = promoCode;
    newBooking.status = "Confirmed";

    const Movie* movie = movieManager.getMovieById(movieId);
    if (!movie) {
        std::cerr << "Error: Movie not found.\n";
        throw std::runtime_error("Invalid movie ID.");
    }

    float ticketPrice = movie->ticketPrice;
    newBooking.ticketPrice = ticketPrice;

    float baseAmount = ticketPrice * seats.size();
    float discounted = applyDiscount(baseAmount, promoCode);
    float taxed = discounted + calculateTax(discounted);
    float finalAmount = taxed + getServiceCharge(theater);

    newBooking.totalAmount = finalAmount;

    bookings.push_back(newBooking);
    saveBookingsToFile();
    return newBooking;
}


bool BookingManager::cancelBooking(long bookingId) {
    for (auto& booking : bookings) {
        if (booking.b_id == bookingId && booking.status != "Cancelled") {
            booking.status = "Cancelled";
            saveBookingsToFile();
            return true;
        }
    }
    return false;
}

bool BookingManager::checkInBooking(long bookingId) {
    for (auto& booking : bookings) {
        if (booking.b_id == bookingId && booking.status == "Confirmed") {
            booking.status = "Checked-In";
            saveBookingsToFile();
            return true;
        }
    }
    return false;
}

std::vector<Booking> BookingManager::getUserBookings(long userId) {
    std::vector<Booking> result;
    for (const auto& booking : bookings) {
        if (booking.u_id == userId) result.push_back(booking);
    }
    return result;
}

std::vector<Booking> BookingManager::getBookingsByUser(long userId) const {
    std::vector<Booking> result;
    for (const auto& booking : bookings) {
        if (booking.u_id == userId) result.push_back(booking);
    }
    return result;
}

Booking* BookingManager::getBookingById(long bookingId) {
    for (auto& booking : bookings) {
        if (booking.b_id == bookingId) return &booking;
    }
    return nullptr;
}

bool BookingManager::modifyBooking(long bookingId, const std::string& newTiming, const std::string& newSeat) {
    for (auto& booking : bookings) {
        if (booking.b_id == bookingId && booking.status == "Confirmed") {
            if (!isModificationAllowed(booking.showTime)) {
                std::cout << "Modification not allowed within 5 hours of showtime.\n";
                return false;
            }
            booking.showTime = newTiming;
            booking.seats.clear();
            booking.seats.push_back(newSeat);  // Assuming only one seat for now
            saveBookingsToFile();
            return true;
        }
    }
    return false;
}

bool BookingManager::isModificationAllowed(const std::string& showTime) const {
    std::tm show_tm{};
    std::istringstream ss(showTime);
    ss >> std::get_time(&show_tm, "%Y-%m-%d %H:%M");
    std::time_t showEpoch = std::mktime(&show_tm);
    std::time_t now = std::time(nullptr);
    double hoursLeft = difftime(showEpoch, now) / 3600.0;
    return hoursLeft > 5.0;
}

long BookingManager::generateUniqueBookingId() const {
    long maxId = 0;
    for (const auto& booking : bookings) {
        if (booking.b_id > maxId) maxId = booking.b_id;
    }
    return maxId + 1;
}

std::string BookingManager::getCurrentDate() const {
    std::ostringstream oss;
    std::time_t now = std::time(nullptr);
    std::tm* tm_ptr = std::localtime(&now);
    oss << std::put_time(tm_ptr, "%Y-%m-%d");
    return oss.str();
}

float BookingManager::calculateTax(float baseAmount) const {
    return baseAmount * 0.18f;  
}

float BookingManager::applyDiscount(float amount, const std::string& promoCode) const {
    if (promoManager.isPromoValid(promoCode)) {
        float discount = promoManager.getDiscountValue(promoCode);
        promoManager.consumePromo(promoCode); 
        return amount - (amount * discount / 100);
    }
    return amount;
}


float BookingManager::getServiceCharge(const std::string& theaterName) const {
    if (theaterName == "IMAX") return 50.0f;
    return 25.0f;
}

void BookingManager::loadBookingsFromFile() {
    bookings = FileIO::loadBookings();
}

void BookingManager::saveBookingsToFile() const {
    FileIO::saveBookings(bookings);
}
