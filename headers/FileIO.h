#ifndef FILE_IO_H
#define FILE_IO_H

#include "MovieBookingStructs.h"
#include <vector>
#include <string>

class FileIO {
public:
    static std::vector<User> loadUsers(const std::string& filename);
    static void saveUsers(const std::string& filename, const std::vector<User>& users);

    static std::vector<Movie> loadMovies(const std::string& filename);
    static void saveMovies(const std::string& filename, const std::vector<Movie>& movies);

    static std::vector<Booking> loadBookings(const std::string& filename);
    static void saveBookings(const std::string& filename, const std::vector<Booking>& bookings);

    static std::vector<MovieTheater> loadTheaters(const std::string& filename);
    static void saveTheaters(const std::string& filename, const std::vector<MovieTheater>& theaters);

    static std::vector<PromoCode> loadPromos(const std::string& filename);
    static void savePromos(const std::string& filename, const std::vector<PromoCode>& promos);

    static Analytics loadAnalytics(const std::string& filename);
    static void saveAnalytics(const std::string& filename, const Analytics& analytics);

private:
    static std::string serializeUser(const User& user);
    static User deserializeUser(const std::string& line);

    static std::string serializeMovie(const Movie& movie);
    static Movie deserializeMovie(const std::string& line);

    static std::string serializeBooking(const Booking& booking);
    static Booking deserializeBooking(const std::string& line);

    static std::string serializeTheater(const MovieTheater& theater);
    static MovieTheater deserializeTheater(const std::string& line);

    static std::string serializePromo(const PromoCode& promo);
    static PromoCode deserializePromo(const std::string& line);

    static std::string serializeAnalytics(const Analytics& data);
    static Analytics deserializeAnalytics(const std::string& line);
};

#endif
