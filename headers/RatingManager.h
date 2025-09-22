#ifndef RATING_MANAGER_H
#define RATING_MANAGER_H

#include "MovieBookingStructs.h"
#include "MovieManager.h"
#include <string>
#include <map>

class RatingManager {
public:
    RatingManager(MovieManager& movieMgr);

    void addRating(long movieId, long userId, int rating, const std::string& comment);
    void viewRatings(long movieId);

private:
    MovieManager& movieManager;

    float calculateAverage(const std::map<long, int>& ratings);
};

#endif
