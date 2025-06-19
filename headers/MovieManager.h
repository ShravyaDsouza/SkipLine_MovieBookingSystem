#ifndef MOVIE_MANAGER_H
#define MOVIE_MANAGER_H

#include "MovieBookingStructs.h"
#include <vector>
#include <string>

class MovieManager {
public:
    MovieManager();

    std::vector<Movie> getAllMovies() const;
    Movie* getMovieById(long movieId);
    std::vector<Movie> searchMoviesByGenre(const std::string& genre);
    std::vector<Movie> searchMoviesByLanguage(const std::string& language);

    void addMovie(const Movie& movie);
    void loadMoviesFromFile();
    void saveMoviesToFile();

private:
    std::vector<Movie> movies;
};

#endif
