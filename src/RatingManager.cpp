#include "RatingManager.h"
#include "FileIO.h"
#include <iostream>
#include <numeric>

RatingManager::RatingManager(MovieManager& movieMgr) : movieManager(movieMgr) {}

void RatingManager::addRating(long movieId, long userId, int rating, const std::string& comment) {
    Movie* movie = movieManager.getMovieById(movieId);
    if (!movie) {
        std::cout << "Movie not found!\n";
        return;
    }

    movie->ratingInfo.userRatings[userId] = rating;
    movie->ratingInfo.userComments[userId] = comment;
    movie->ratingInfo.avgRating = calculateAverage(movie->ratingInfo.userRatings);

    movieManager.saveMoviesToFile();
    std::cout << "Rating and comment submitted successfully.\n";
}

void RatingManager::viewRatings(long movieId) {
    Movie* movie = movieManager.getMovieById(movieId);
    if (!movie) {
        std::cout << "Movie not found!\n";
        return;
    }

    std::cout << "\nRatings for " << movie->name << " (Avg: " << movie->ratingInfo.avgRating << "):\n";
    for (const auto& [userId, rating] : movie->ratingInfo.userRatings) {
        std::cout << "User ID: " << userId << " | Rating: " << rating;
        if (movie->ratingInfo.userComments.count(userId)) {
            std::cout << " | Comment: " << movie->ratingInfo.userComments[userId];
        }
        std::cout << "\n";
    }
}

float RatingManager::calculateAverage(const std::map<long, int>& ratings) {
    if (ratings.empty()) return 0.0f;
    float total = std::accumulate(ratings.begin(), ratings.end(), 0.0f,
        [](float sum, const auto& pair) { return sum + pair.second; });
    return total / ratings.size();
}
