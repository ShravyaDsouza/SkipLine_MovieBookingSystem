#ifndef ANALYTICS_MANAGER_H
#define ANALYTICS_MANAGER_H

#include "MovieBookingStructs.h"
#include <map>
#include <string>

class AnalyticsManager {
public:
    AnalyticsManager();

    void updateAnalytics(const Booking& booking);
    void printRevenuePerTheater() const;
    void printTicketsSoldPerMovie() const;

    void loadAnalyticsFromFile();
    void saveAnalyticsToFile();

private:
    Analytics analytics;
};

#endif
