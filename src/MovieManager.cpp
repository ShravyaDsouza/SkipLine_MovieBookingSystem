#include "MovieManager.h"
#include "FileIO.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

MovieManager::MovieManager() {
    loadMoviesFromFile();
}

std::vector<Movie> MovieManager::getAllMovies() const {
    return movies;
}

std::optional<Movie> MovieManager::getMovieById(long movieId) const {
    for (const auto& movie : movies) {
        if (movie.m_id == movieId) return movie;
    }
    return std::nullopt;
}

std::vector<Movie> MovieManager::searchMoviesByGenre(const std::string& genre) const {
    std::vector<Movie> result;
    for (const auto& movie : movies) {
        if (movie.genre == genre) result.push_back(movie);
    }
    return result;
}

std::vector<Movie> MovieManager::searchMoviesByLanguage(const std::string& language) const {
    std::vector<Movie> result;
    for (const auto& movie : movies) {
        if (movie.language == language) result.push_back(movie);
    }
    return result;
}

std::vector<Movie> MovieManager::searchMoviesByName(const std::string& keyword) const {
    std::vector<Movie> result;
    for (const auto& movie : movies) {
        if (movie.name.find(keyword) != std::string::npos) {
            result.push_back(movie);
        }
    }
    return result;
}

std::vector<Movie> MovieManager::filterMoviesByTiming(const std::string& timing) const {
    std::vector<Movie> result;
    for (const auto& movie : movies) {
        if (movie.seatsRemainingPerTiming.count(timing)) {
            result.push_back(movie);
        }
    }
    return result;
}

std::vector<Movie> MovieManager::filterMoviesByTheaterLocation(const std::string& location, const TheaterManager& theaterManager) const {
    std::vector<Movie> result;

    for (const auto& movie : movies) {
        for (const auto& theaterName : movie.theatersAvailable) {
            const MovieTheater* theater = theaterManager.getTheaterByName(theaterName);
            if (theater && theater->location == location) {
                result.push_back(movie);
                break;
            }
        }
    }

    return result;
}

std::vector<Movie> MovieManager::sortByRating(bool descending) const {
    std::vector<Movie> sorted = movies;
    std::sort(sorted.begin(), sorted.end(), [&](const Movie& a, const Movie& b) {
        return descending ? a.ratingInfo.avgRating > b.ratingInfo.avgRating
                          : a.ratingInfo.avgRating < b.ratingInfo.avgRating;
    });
    return sorted;
}

void MovieManager::addMovie(const Movie& movie) {
    movies.push_back(movie);
    saveMoviesToFile();
}

void MovieManager::updateMovie(long movieId, const Movie& updatedMovie) {
    for (auto& movie : movies) {
        if (movie.m_id == movieId) {
            movie = updatedMovie;
            break;
        }
    }
    saveMoviesToFile();
}

void MovieManager::deleteMovie(long movieId) {
    movies.erase(std::remove_if(movies.begin(), movies.end(),
        [movieId](const Movie& m) { return m.m_id == movieId; }), movies.end());
    saveMoviesToFile();
}

void MovieManager::updateSeatsAfterBooking(long movieId, const std::string& timing, int bookedSeats) {
    for (auto& movie : movies) {
        if (movie.m_id == movieId) {
            if (movie.seatsRemainingPerTiming.count(timing)) {
                movie.seatsRemainingPerTiming[timing] -= bookedSeats;
            }
            break;
        }
    }
    saveMoviesToFile();
}

void MovieManager::setSeatPricing(long movieId, map<string, float> seatTypeToPrice) {
    for (auto& movie : movies) {
        if (movie.m_id == movieId) {
            movie.seatTypeToPrice = seatTypeToPrice;
            saveMoviesToFile();
            return;
        }
    }
}

void MovieManager::updateRating(long movieId, long userId, int rating, const std::string& comment) {
    for (auto& movie : movies) {
        if (movie.m_id == movieId) {
            movie.ratingInfo.userRatings[userId] = rating;
            movie.ratingInfo.userComments[userId] = comment;

            float sum = 0.0;
            for (const auto& pair : movie.ratingInfo.userRatings) {
                sum += pair.second;
            }
            movie.ratingInfo.avgRating = sum / movie.ratingInfo.userRatings.size();
            break;
        }
    }
    saveMoviesToFile();
}

long MovieManager::generateUniqueMovieId() const {
    long maxId = 0;
    for (const auto& movie : movies) {
        if (movie.m_id > maxId) maxId = movie.m_id;
    }
    return maxId + 1;
}

void MovieManager::loadMoviesFromFile() {
    movies = FileIO::loadMovies("data/movies.txt");
}

void MovieManager::saveMoviesToFile() const {
    FileIO::saveMovies("data/movies.txt", movies);
}

void MovieManager::importMoviesFromCSV(const std::string& filePath) {
    std::ifstream file(filePath);
    std::string line;
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return;
    }

    getline(file, line);

    while (getline(file, line)) {
        std::stringstream ss(line);
        Movie m;
        std::string temp;

        getline(ss, m.name, ',');
        getline(ss, m.genre, ',');
        getline(ss, m.language, ',');
        getline(ss, temp, ','); m.duration = stoi(temp);
        getline(ss, temp, ','); m.m_id = generateUniqueMovieId();

        movies.push_back(m);
    }

    file.close();
    saveMoviesToFile();
}

void MovieManager::exportMoviesToCSV(const std::string& filePath) const {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error opening file for writing: " << filePath << std::endl;
        return;
    }

    file << "ID,Name,Genre,Language,Duration,AvgRating\n";

    for (const auto& m : movies) {
        file << m.m_id << "," << m.name << "," << m.genre << ","
             << m.language << "," << m.duration << "," << m.ratingInfo.avgRating << "\n";
    }

    file.close();
}

void MovieManager::viewAllRatingsAndComments(long movieId) const {
    for (const auto& m : movies) {
        if (m.m_id == movieId) {
            std::cout << "\nRatings and Comments for: " << m.name << "\n";
            for (const auto& [uid, rating] : m.ratingInfo.userRatings) {
                std::string comment = m.ratingInfo.userComments.at(uid);
                std::cout << "User " << uid << ": " << rating << " stars — \"" << comment << "\"\n";
            }
            return;
        }
    }
    std::cout << "Movie not found.\n";
}

void MovieManager::moderateReview(long movieId, long userId) {
    for (auto& m : movies) {
        if (m.m_id == movieId) {
            if (m.ratingInfo.userRatings.count(userId)) {
                m.ratingInfo.userRatings.erase(userId);
                m.ratingInfo.userComments.erase(userId);
                std::cout << "Review removed successfully.\n";

                float sum = 0.0f;
                for (const auto& r : m.ratingInfo.userRatings)
                    sum += r.second;
                m.ratingInfo.avgRating = m.ratingInfo.userRatings.empty() ? 0.0f : sum / m.ratingInfo.userRatings.size();

                saveMoviesToFile();
            } else {
                std::cout << "No review by this user found.\n";
            }
            return;
        }
    }
    std::cout << "Movie not found.\n";
}

