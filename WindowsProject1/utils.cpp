#include "utils.h"

// TODO: Define all the declared functions

int getPricePerDay(Genre g) {
    return 0; // Placeholder
}

int calculateLateFee(const std::chrono::system_clock::time_point& due,
    const std::chrono::system_clock::time_point& returned) {
    return 0; // Placeholder
}

std::string genreToString(Genre g) {
    switch (g) {
        case Genre::Action: return "Action";
        case Genre::Comedy: return "Comedy";
        case Genre::Drama: return "Drama";
        default: return "Undefined";
    }
}

Genre stringToGenre(const std::string& s) {
    if (s == "Action") return Genre::Action;
    if (s == "Comedy") return Genre::Comedy;
    if (s == "Drama") return Genre::Drama;
    return Genre::Undefined;
}

// Check if a due date is overdue
bool isOverdue(const std::chrono::system_clock::time_point& dueDate) {
    auto now = std::chrono::system_clock::now();
    return now > dueDate;
}


