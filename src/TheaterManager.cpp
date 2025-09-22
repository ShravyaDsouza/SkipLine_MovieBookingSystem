#include "TheaterManager.h"
#include "FileIO.h"
#include <iostream>
#include <algorithm>
using namespace std;

void TheaterManager::loadTheatersFromFile() {
    theaters = FileIO::loadTheaters("data/theaters.txt");
}

void TheaterManager::saveTheatersToFile() {
    FileIO::saveTheaters("data/theaters.txt", theaters);
}

TheaterManager::TheaterManager() {
    loadTheatersFromFile();
}

void TheaterManager::addTheater(const MovieTheater& theater) {
    if (theaterExists(theater.name)) {
        std::cout << "Theater already exists: " << theater.name << "\n";
        return;
    }
    theaters.push_back(theater);
    saveTheatersToFile();
    std::cout << "Theater added successfully.\n";
}

std::vector<MovieTheater> TheaterManager::getAllTheaters() const {
    return theaters;
}

MovieTheater* TheaterManager::getTheaterByName(const std::string& name) {
    for (auto& theater : theaters) { 
        if (theater.name == name) return &theater;
    }
    return nullptr;
}

bool TheaterManager::theaterExists(const std::string& name) const {
    return std::any_of(theaters.begin(), theaters.end(), [&](const MovieTheater& t) {
        return t.name == name;
    });
}

void TheaterManager::updateTheaterDetails(const std::string& name, const MovieTheater& updatedData) {
    for (auto& theater : theaters) {
        if (theater.name == name) {
            theater = updatedData;
            saveTheatersToFile();
            std::cout << "Theater details updated successfully.\n";
            return;
        }
    }
    std::cout << "Theater not found: " << name << "\n";
}

bool TheaterManager::deleteTheater(const std::string& name) {
    auto it = std::remove_if(theaters.begin(), theaters.end(), [&](const MovieTheater& t) {
        return t.name == name;
    });

    if (it != theaters.end()) {
        theaters.erase(it, theaters.end());
        saveTheatersToFile();
        return true;
    } else {
        return false;
    }
}

bool TheaterManager::isSeatAvailable(const std::string& theaterName, const std::string& showTime, const std::string& seat){
    MovieTheater* theater = getTheaterByName(theaterName);
    if (!theater) return false;

    return theater->seatAvailabilityPerShowTime.count(showTime) &&
           theater->seatAvailabilityPerShowTime.at(showTime).count(seat) &&
           theater->seatAvailabilityPerShowTime.at(showTime).at(seat);
}

void TheaterManager::reserveSeats(const std::string& theaterName, const std::string& showTime, const std::vector<std::string>& seats) {
    MovieTheater* theater = getTheaterByName(theaterName);
    if (!theater) return;

    for (const auto& seat : seats) {
        theater->seatAvailabilityPerShowTime[showTime][seat] = false;
    }
    saveTheatersToFile();
}

void TheaterManager::releaseSeats(const std::string& theaterName, const std::string& showTime, const std::vector<std::string>& seats) {
    MovieTheater* theater = getTheaterByName(theaterName);
    if (!theater) {
        std::cout << "Theater not found: " << theaterName << "\n";
        return;
    }

    if (!theater->seatAvailabilityPerShowTime.count(showTime)) {
        std::cout << "Show time not found: " << showTime << " in theater " << theaterName << "\n";
        return;
    }

    for (const auto& seat : seats) {
        if (theater->seatAvailabilityPerShowTime[showTime].count(seat)) {
            theater->seatAvailabilityPerShowTime[showTime][seat] = true;  
        } else {
            std::cout << "Seat \"" << seat << "\" not found in show time " << showTime << "\n";
        }
    }

    saveTheatersToFile();
    std::cout << "Selected seats released for show \"" << showTime << "\" in \"" << theaterName << "\".\n";
}

float TheaterManager::getServiceCharge(const std::string& theaterName) const {
    for (const auto& theater : theaters) {
        if (theater.name == theaterName) {
            return theater.serviceCharge;
        }
    }
    std::cerr << "Theater \"" << theaterName << "\" not found. Returning default service charge 0.0\n";
    return 0.0f;
}

std::vector<std::string> TheaterManager::getShowTimings(const std::string& theaterName) {
    std::vector<std::string> timings;
    MovieTheater* theater = getTheaterByName(theaterName);
    if (theater) {
        for (const auto& pair : theater->seatAvailabilityPerShowTime) {
            timings.push_back(pair.first);
        }
    }
    return timings;
}

std::vector<MovieTheater> TheaterManager::searchTheatersByLocation(const std::string& location) const {
    std::vector<MovieTheater> result;
    for (const auto& theater : theaters) {
        if (theater.location == location) {
            result.push_back(theater);
        }
    }
    return result;
}
