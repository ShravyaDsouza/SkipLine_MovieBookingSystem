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

    bool isSeatAvailable(const std::string& theaterName, const std::string& showTime, const std::string& seat);
    void reserveSeats(const std::string& theaterName, const std::string& showTime, const std::vector<std::string>& seats);
    void releaseSeats(const std::string& theaterName, const std::string& showTime, const std::vector<std::string>& seats);

    void addTheater(const MovieTheater& theater);
    void loadTheatersFromFile();
    void saveTheatersToFile();

private:
    std::vector<MovieTheater> theaters;
};

#endif
