#include "FileIO.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <iomanip>

// --- USERS ---

std::vector<User> FileIO::loadUsers(const std::string& filename) {
    std::vector<User> users;
    std::ifstream infile(filename);
    std::string line;

    while (getline(infile, line)) {
        users.push_back(deserializeUser(line));
    }

    infile.close();
    return users;
}

void FileIO::saveUsers(const std::string& filename, const std::vector<User>& users) {
    std::ofstream outfile(filename);
    for (const auto& user : users) {
        outfile << serializeUser(user) << "\n";
    }
    outfile.close();
}

// --- Internal Helper ---

std::string FileIO::serializeUser(const User& user) {
    std::ostringstream oss;
    oss << user.u_id << ","
        << user.type << ","
        << user.phoneno << ","
        << user.name << ","
        << user.email << ","
        << user.hashPassword << ","
        << user.isAdmin;

    // bookingHistory can be stored later if needed
    return oss.str();
}

User FileIO::deserializeUser(const std::string& line) {
    std::istringstream iss(line);
    std::string token;
    User user;

    getline(iss, token, ','); user.u_id = stol(token);
    getline(iss, user.type, ',');
    getline(iss, user.phoneno, ',');
    getline(iss, user.name, ',');
    getline(iss, user.email, ',');
    getline(iss, user.hashPassword, ',');
    getline(iss, token, ','); user.isAdmin = (token == "1");

    return user;
}

// -- Theater Manager ---
std::vector<MovieTheater> FileIO::loadTheaters(const std::string& filename) {
    std::vector<MovieTheater> theaters;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << "\n";
        return theaters;
    }

    int theaterCount;
    file >> theaterCount;
    file.ignore();

    for (int t = 0; t < theaterCount; ++t) {
        MovieTheater theater;

        std::getline(file, theater.name);
        std::getline(file, theater.location);
        std::getline(file, theater.pincode);
        file >> theater.totalSeats;
        file.ignore();

        // Read seat layout
        int layoutSize;
        file >> layoutSize;
        file.ignore();
        for (int i = 0; i < layoutSize; ++i) {
            std::string seat;
            std::getline(file, seat);
            theater.seatLayout.push_back(seat);
        }

        file >> theater.serviceCharge;
        file.ignore();

        // Read seat availability
        int showTimeCount;
        file >> showTimeCount;
        file.ignore();
        for (int i = 0; i < showTimeCount; ++i) {
            std::string showTime;
            std::getline(file, showTime);
            int seatCount;
            file >> seatCount;
            file.ignore();
            for (int j = 0; j < seatCount; ++j) {
                std::string seat;
                bool available;
                file >> seat >> available;
                theater.seatAvailabilityPerShowTime[showTime][seat] = available;
            }
            file.ignore();
        }

        theaters.push_back(theater);
    }

    file.close();
    return theaters;
}

void FileIO::saveTheaters(const std::string& filename, const std::vector<MovieTheater>& theaters) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open file to save: " << filename << "\n";
        return;
    }

    file << theaters.size() << "\n";
    for (const auto& theater : theaters) {
        file << theater.name << "\n";
        file << theater.location << "\n";
        file << theater.pincode << "\n";
        file << theater.totalSeats << "\n";

        file << theater.seatLayout.size() << "\n";
        for (const auto& seat : theater.seatLayout) {
            file << seat << "\n";
        }

        file << theater.serviceCharge << "\n";

        file << theater.seatAvailabilityPerShowTime.size() << "\n";
        for (const auto& [showTime, seats] : theater.seatAvailabilityPerShowTime) {
            file << showTime << "\n";
            file << seats.size() << "\n";
            for (const auto& [seat, available] : seats) {
                file << seat << " " << available << "\n";
            }
        }
    }

    file.close();
}

// -- Movie --
std::string removeQuotes(const std::string& s) {
    if (s.size() >= 2 && s.front() == '"' && s.back() == '"')
        return s.substr(1, s.size() - 2);
    return s;
}

std::string FileIO::serializeMovie(const Movie& movie) {
    std::ostringstream oss;
    oss << movie.m_id << ","
        << std::quoted(movie.name) << ","
        << std::quoted(movie.genre) << ","
        << std::quoted(movie.language) << ","
        << movie.duration << ","
        << movie.ticketPrice << ","
        << std::quoted(movie.seat_type) << ","
        << std::quoted(movie.description) << ","
        << movie.ageLimit << ","
        << std::quoted(movie.releaseDate);

    oss << "," << movie.showTimings.size();
    for (const auto& t : movie.showTimings)
        oss << "," << std::quoted(t);

    oss << "," << movie.seatsRemainingPerTiming.size();
    for (const auto& [time, seat] : movie.seatsRemainingPerTiming)
        oss << "," << std::quoted(time) << "," << seat;

    oss << "," << movie.seatTypeToPrice.size();
    for (const auto& [type, price] : movie.seatTypeToPrice)
        oss << "," << std::quoted(type) << "," << price;

    oss << "," << movie.theatersAvailable.size();
    for (const auto& th : movie.theatersAvailable)
        oss << "," << std::quoted(th);

    oss << "," << movie.ratingInfo.avgRating << "," << movie.ratingInfo.userRatings.size();
    for (const auto& [uid, rating] : movie.ratingInfo.userRatings)
        oss << "," << uid << "," << rating;

    oss << "," << movie.ratingInfo.userComments.size();
    for (const auto& [uid, comment] : movie.ratingInfo.userComments)
        oss << "," << uid << "," << std::quoted(comment);

    return oss.str();
}

Movie FileIO::deserializeMovie(const std::string& line) {
    std::istringstream iss(line);
    Movie movie;
    std::string temp;

    std::getline(iss, temp, ','); movie.m_id = std::stol(temp);
    std::getline(iss, temp, ','); movie.name = removeQuotes(temp);
    std::getline(iss, temp, ','); movie.genre = removeQuotes(temp);
    std::getline(iss, temp, ','); movie.language = removeQuotes(temp);
    std::getline(iss, temp, ','); movie.duration = std::stof(temp);
    std::getline(iss, temp, ','); movie.ticketPrice = std::stof(temp);
    std::getline(iss, temp, ','); movie.seat_type = removeQuotes(temp);
    std::getline(iss, temp, ','); movie.description = removeQuotes(temp);
    std::getline(iss, temp, ','); movie.ageLimit = std::stoi(temp);
    std::getline(iss, temp, ','); movie.releaseDate = removeQuotes(temp);

    size_t size;
    std::getline(iss, temp, ','); size = std::stoul(temp);
    for (size_t i = 0; i < size; ++i) {
        std::getline(iss, temp, ',');
        movie.showTimings.insert(removeQuotes(temp));
    }

    std::getline(iss, temp, ','); size = std::stoul(temp);
    for (size_t i = 0; i < size; ++i) {
        std::string time;
        std::getline(iss, time, ',');
        std::getline(iss, temp, ',');
        movie.seatsRemainingPerTiming[removeQuotes(time)] = std::stoi(temp);
    }

    std::getline(iss, temp, ','); size = std::stoul(temp);
    for (size_t i = 0; i < size; ++i) {
        std::string type;
        std::getline(iss, type, ',');
        std::getline(iss, temp, ',');
        movie.seatTypeToPrice[removeQuotes(type)] = std::stof(temp);
    }

    std::getline(iss, temp, ','); size = std::stoul(temp);
    for (size_t i = 0; i < size; ++i) {
        std::getline(iss, temp, ',');
        movie.theatersAvailable.push_back(removeQuotes(temp));
    }

    std::getline(iss, temp, ','); movie.ratingInfo.avgRating = std::stof(temp);
    std::getline(iss, temp, ','); size = std::stoul(temp);
    for (size_t i = 0; i < size; ++i) {
        long uid; int r;
        std::getline(iss, temp, ','); uid = std::stol(temp);
        std::getline(iss, temp, ','); r = std::stoi(temp);
        movie.ratingInfo.userRatings[uid] = r;
    }

    std::getline(iss, temp, ','); size = std::stoul(temp);
    for (size_t i = 0; i < size; ++i) {
        long uid;
        std::string comment;
        std::getline(iss, temp, ','); uid = std::stol(temp);
        std::getline(iss, comment, ',');
        movie.ratingInfo.userComments[uid] = removeQuotes(comment);
    }

    return movie;
}

std::vector<Movie> FileIO::loadMovies(const std::string& filename) {
    std::vector<Movie> movies;
    std::ifstream in(filename);
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty()) {
            movies.push_back(deserializeMovie(line));
        }
    }
    return movies;
}

void FileIO::saveMovies(const std::string& filename, const std::vector<Movie>& movies) {
    std::ofstream out(filename);
    for (const auto& m : movies) {
        out << serializeMovie(m) << "\n";
    }
}