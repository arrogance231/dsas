#ifndef UTILS_HPP
#define UTILS_HPP

#include <chrono>
#include <string>

enum class Genre {
    Action,
    Comedy,
    Drama,
    Undefined
};

// Fee system helpers
int getPricePerDay(Genre g);
int calculateLateFee(const std::chrono::system_clock::time_point& due,
    const std::chrono::system_clock::time_point& returned);

// Optional string <-> enum helpers
std::string genreToString(Genre g);
Genre stringToGenre(const std::string& s);

#endif
