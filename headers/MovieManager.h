#ifndef MOVIE_MANAGER_H
#define MOVIE_MANAGER_H

#include "MovieBookingStructs.h"
#include "TheaterManager.h"
#include <vector>
#include <string>
#include <optional>

class MovieManager {
public:
    MovieManager();

    std::vector<Movie> getAllMovies() const;
    std::optional<Movie> getMovieById(long movieId) const;
    std::vector<Movie> searchMoviesByGenre(const std::string& genre) const;
    std::vector<Movie> searchMoviesByLanguage(const std::string& language) const;
    std::vector<Movie> searchMoviesByName(const std::string& keyword) const;
    std::vector<Movie> filterMoviesByTiming(const std::string& timing) const;
    std::vector<Movie> filterMoviesByTheaterLocation(const std::string& location, const TheaterManager& theaterManager) const;
    std::vector<Movie> sortByRating(bool descending = true) const;

    void addMovie(const Movie& movie);
    void updateMovie(long movieId, const Movie& updatedMovie);
    void deleteMovie(long movieId);

    void updateSeatsAfterBooking(long movieId, const std::string& timing, int bookedSeats);
    void setSeatPricing(long movieId,map<string, float> seatTypeToPrice);
    void updateRating(long movieId, long userId, int rating, const std::string& comment);

    void loadMoviesFromFile();
    void saveMoviesToFile() const;
    void importMoviesFromCSV(const std::string& filePath);
    void exportMoviesToCSV(const std::string& filePath) const;

    void viewAllRatingsAndComments(long movieId) const;
    void moderateReview(long movieId, long userId);

    long generateUniqueMovieId() const;

private:
    std::vector<Movie> movies;
};

#endif
