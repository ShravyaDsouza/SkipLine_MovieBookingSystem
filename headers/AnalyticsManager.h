#ifndef ANALYTICS_MANAGER_H
#define ANALYTICS_MANAGER_H

#include "MovieBookingStructs.h"
#include <map>
#include <string>
#include <ctime>
#include <vector>

class AnalyticsManager {
public:
    AnalyticsManager();

    void updateAnalytics(const Booking& booking, const Movie& movie);

    void printRevenuePerTheater() const;
    void printTicketsSoldPerMovie(const std::vector<Movie>& allMovies) const;
    void displayTopMoviesByBookings(int topN, const std::vector<Movie>& allMovies) const;
    void displayRevenueReport() const;
    void printBookingsPerDay() const;

    void loadAnalyticsFromFile();
    void saveAnalyticsToFile();
    void reset();

    Analytics getAnalytics() const;

private:
    Analytics data;

    std::string serializeAnalytics() const;
    void deserializeAnalytics(const std::string& line);
};

#endif
