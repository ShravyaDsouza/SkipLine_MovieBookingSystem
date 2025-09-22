#ifndef THEATER_MANAGER_H
#define THEATER_MANAGER_H

#include "MovieBookingStructs.h"
#include <string>
#include <vector>
#include <map>

class TheaterManager {
public:
    TheaterManager();

    std::vector<MovieTheater> getAllTheaters() const;
    MovieTheater* getTheaterByName(const std::string& name);
    bool theaterExists(const std::string& name) const;

    bool isSeatAvailable(const std::string& theaterName, const std::string& showTime, const std::string& seat);
    void reserveSeats(const std::string& theaterName, const std::string& showTime, const std::vector<std::string>& seats);
    void releaseSeats(const std::string& theaterName, const std::string& showTime, const std::vector<std::string>& seats);

    void addTheater(const MovieTheater& theater);
    void updateTheaterDetails(const std::string& name, const MovieTheater& updatedData);
    bool deleteTheater(const std::string& theaterName);

    void loadTheatersFromFile();
    void saveTheatersToFile();

    float getServiceCharge(const std::string& theaterName) const;

    std::vector<MovieTheater> searchTheatersByLocation(const std::string& location) const;
    std::vector<std::string> getShowTimings(const std::string& theaterName);

private:
    std::vector<MovieTheater> theaters;
};

#endif
