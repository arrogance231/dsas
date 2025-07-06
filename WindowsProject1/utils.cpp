#include "utils.h"

// TODO: Define all the declared functions

int getPricePerDay(Genre g) {
    return 300;
}

int calculateLateFee(const std::chrono::system_clock::time_point& due,
    const std::chrono::system_clock::time_point& returned) {
    if (returned <= due) return 0;
    int lateDays = std::chrono::duration_cast<std::chrono::hours>(returned - due).count() / 24;
    if ((std::chrono::duration_cast<std::chrono::hours>(returned - due).count() % 24) > 0) lateDays++;
    int lateFeePerDay = static_cast<int>(300 * 0.3);
    return lateDays * lateFeePerDay;
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


