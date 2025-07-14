#include "core.h"
#include <vector>
#include <chrono>
#include <ctime>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <cstdio>

// Simple JSON implementation
class SimpleJson {
public:
    static std::string escapeString(const std::string& str) {
        std::string result;
        for (char c : str) {
            if (c == '"' || c == '\\') {
                result += '\\';
            }
            result += c;
        }
        return result;
    }
    
    static std::string writeMovie(const Movie& movie) {
        std::stringstream ss;
        ss << "{\n";
        ss << "  \"id\": " << movie.id << ",\n";
        ss << "  \"title\": \"" << escapeString(movie.title) << "\",\n";
        ss << "  \"genre\": " << static_cast<int>(movie.genre) << ",\n";
        ss << "  \"copiesAvailable\": " << movie.copiesAvailable << ",\n";
        ss << "  \"releaseDate\": \"" << escapeString(movie.releaseDate) << "\",\n";
        ss << "  \"description\": \"" << escapeString(movie.description) << "\",\n";
        ss << "  \"coverImagePath\": \"" << escapeString(movie.coverImagePath) << "\"\n";
        ss << "}";
        return ss.str();
    }
    
    static bool parseMovie(const std::string& jsonStr, Movie& movie) {
        // Simple JSON parser - this is a basic implementation
        std::istringstream iss(jsonStr);
        std::string line;
        
        while (std::getline(iss, line)) {
            if (line.find("\"id\":") != std::string::npos) {
                size_t pos = line.find(":");
                if (pos != std::string::npos) {
                    movie.id = std::stoi(line.substr(pos + 1));
                }
            } else if (line.find("\"title\":") != std::string::npos) {
                size_t start = line.find("\"", line.find("\"title\":") + 8);
                size_t end = line.find("\"", start + 1);
                if (start != std::string::npos && end != std::string::npos) {
                    movie.title = line.substr(start + 1, end - start - 1);
                }
            } else if (line.find("\"genre\":") != std::string::npos) {
                size_t pos = line.find(":");
                if (pos != std::string::npos) {
                    movie.genre = static_cast<Genre>(std::stoi(line.substr(pos + 1)));
                }
            } else if (line.find("\"copiesAvailable\":") != std::string::npos) {
                size_t pos = line.find(":");
                if (pos != std::string::npos) {
                    movie.copiesAvailable = std::stoi(line.substr(pos + 1));
                }
            } else if (line.find("\"releaseDate\":") != std::string::npos) {
                size_t start = line.find("\"", line.find("\"releaseDate\":") + 13);
                size_t end = line.find("\"", start + 1);
                if (start != std::string::npos && end != std::string::npos) {
                    movie.releaseDate = line.substr(start + 1, end - start - 1);
                }
            } else if (line.find("\"description\":") != std::string::npos) {
                size_t start = line.find("\"", line.find("\"description\":") + 13);
                size_t end = line.find("\"", start + 1);
                if (start != std::string::npos && end != std::string::npos) {
                    movie.description = line.substr(start + 1, end - start - 1);
                }
            } else if (line.find("\"coverImagePath\":") != std::string::npos) {
                size_t start = line.find("\"", line.find("\"coverImagePath\":") + 16);
                size_t end = line.find("\"", start + 1);
                if (start != std::string::npos && end != std::string::npos) {
                    movie.coverImagePath = line.substr(start + 1, end - start - 1);
                }
            }
        }
        return true;
    }
};

// User management
void SystemManager::addUser(const User& user) {
    users[user.username] = user;
    saveToFile("data.txt");
}

bool SystemManager::verifyLogin(const std::string& username, const std::string& password) {
    auto it = users.find(username);
    if (it != users.end() && it->second.password == password) {
        currentUser = &it->second;
        return true;
    }
    return false;
}

User* SystemManager::getCurrentUser() {
    return currentUser;
}

void SystemManager::logout() {
    currentUser = nullptr;
}

// Customer management
void SystemManager::addCustomer(const Customer& c) {
    customers[c.id] = c;
    saveToFile("data.txt");
}

void SystemManager::editCustomer(int customerID, const Customer& c) {
    if (customers.count(customerID)) {
        customers[customerID] = c;
        saveToFile("data.txt");
    }
}

void SystemManager::removeCustomer(int customerID) {
    customers.erase(customerID);
    saveToFile("data.txt");
}

Customer* SystemManager::getCustomer(int customerID) {
    auto it = customers.find(customerID);
    if (it != customers.end()) {
        return &it->second;
    }
    return nullptr;
}

std::vector<Customer> SystemManager::getAllCustomers() {
    std::vector<Customer> allCustomers;
    for (auto const& [id, customer] : customers) {
        allCustomers.push_back(customer);
    }
    return allCustomers;
}

// Movie management
void SystemManager::addMovie(const Movie& m) {
    movies[m.id] = m;
    saveToFile("data.txt");
}

void SystemManager::editMovie(int movieID, const Movie& m) {
    if (movies.count(movieID)) {
        movies[movieID] = m;
        saveToFile("data.txt");
    }
}

void SystemManager::removeMovie(int movieID) {
    movies.erase(movieID);
    saveToFile("data.txt");
}

Movie* SystemManager::getMovie(int movieID) {
    auto it = movies.find(movieID);
    if (it != movies.end()) {
        return &it->second;
    }
    return nullptr;
}

std::vector<Movie> SystemManager::getAllMovies() {
    std::vector<Movie> allMovies;
    for (auto const& [id, movie] : movies) {
        allMovies.push_back(movie);
    }
    return allMovies;
}

// Rental operations
void SystemManager::rentMovie(int customerID, int movieID, int rentalDays) {
    // Validate customer exists
    Customer* customer = getCustomer(customerID);
    if (!customer) {
        throw std::runtime_error("Customer not found");
    }

    // Validate movie exists and is available
    Movie* movie = getMovie(movieID);
    if (!movie) {
        throw std::runtime_error("Movie not found");
    }
    if (movie->copiesAvailable <= 0) {
        throw std::runtime_error("No copies available for rental");
    }

    // Create rental record
    RentalRecord record;
    record.customerID = customerID;
    record.movieID = movieID;
    record.rentDate = std::chrono::system_clock::now();
    record.dueDate = record.rentDate + std::chrono::hours(24 * rentalDays);

    // Add to undo stack
    RentalAction action;
    action.actionType = RentalAction::Rent;
    action.record = record;
    undoStack.push(action);

    // Update system state
    movie->copiesAvailable--;
    customer->activeRentals.push(movieID);
    saveToFile("data.txt");
}

void SystemManager::returnMovie(int customerID, int movieID) {
    // Validate customer exists
    Customer* customer = getCustomer(customerID);
    if (!customer) {
        throw std::runtime_error("Customer not found");
    }

    // Validate movie exists
    Movie* movie = getMovie(movieID);
    if (!movie) {
        throw std::runtime_error("Movie not found");
    }

    // Check if customer has this movie rented
    bool hasRental = false;
    std::queue<int> tempQueue = customer->activeRentals;
    while (!tempQueue.empty()) {
        if (tempQueue.front() == movieID) {
            hasRental = true;
            break;
        }
        tempQueue.pop();
    }
    if (!hasRental) {
        throw std::runtime_error("Customer doesn't have this movie rented");
    }

    // Retrieve the original rent and due date from the undo stack
    std::stack<RentalAction> tempStack = getUndoStack();
    std::chrono::system_clock::time_point rentDate, dueDate;
    bool found = false;
    while (!tempStack.empty()) {
        RentalAction action = tempStack.top();
        tempStack.pop();
        if (action.actionType == RentalAction::Rent &&
            action.record.customerID == customerID &&
            action.record.movieID == movieID) {
            rentDate = action.record.rentDate;
            dueDate = action.record.dueDate;
            found = true;
            break;
        }
    }
    if (!found) {
        throw std::runtime_error("Original rental record not found");
    }

    // Create return record
    RentalRecord record;
    record.customerID = customerID;
    record.movieID = movieID;
    record.returnDate = std::chrono::system_clock::now();
    record.dueDate = dueDate;

    // Calculate late fee if applicable
    if (record.returnDate > record.dueDate) {
        auto duration = record.returnDate - record.dueDate;
        auto hoursLate = std::chrono::duration_cast<std::chrono::hours>(duration).count();
        record.lateFee = static_cast<int>(hoursLate) * 1; // $1 per hour late
    }

    // Add to undo stack
    RentalAction action;
    action.actionType = RentalAction::Return;
    action.record = record;
    undoStack.push(action);

    // Update system state
    movie->copiesAvailable++;

    // Update customer's active rentals
    std::queue<int> newActiveRentals;
    while (!customer->activeRentals.empty()) {
        int rentedID = customer->activeRentals.front();
        customer->activeRentals.pop();
        if (rentedID != movieID) {
            newActiveRentals.push(rentedID);
        }
    }
    customer->activeRentals = newActiveRentals;

    // Add to rental history
    customer->rentalHistory.push(movieID);
    saveToFile("data.txt");
}

void SystemManager::undoLastAction() {
    if (undoStack.empty()) {
        throw std::runtime_error("No actions to undo");
    }

    RentalAction lastAction = undoStack.top();
    undoStack.pop();

    switch (lastAction.actionType) {
    case RentalAction::Rent: {
        // Undoing a rental - perform a return
        Customer* customer = getCustomer(lastAction.record.customerID);
        Movie* movie = getMovie(lastAction.record.movieID);

        if (customer && movie) {
            movie->copiesAvailable++;

            // Remove from active rentals
            std::queue<int> newActiveRentals;
            while (!customer->activeRentals.empty()) {
                int rentedID = customer->activeRentals.front();
                customer->activeRentals.pop();
                if (rentedID != lastAction.record.movieID) {
                    newActiveRentals.push(rentedID);
                }
            }
            customer->activeRentals = newActiveRentals;
        }
        break;
    }
    case RentalAction::Return: {
        // Undoing a return - perform a rental
        Customer* customer = getCustomer(lastAction.record.customerID);
        Movie* movie = getMovie(lastAction.record.movieID);

        if (customer && movie) {
            movie->copiesAvailable--;
            customer->activeRentals.push(lastAction.record.movieID);

            // Remove history if added
            std::queue<int> newHistory;
            while (!customer->rentalHistory.empty()) {
                int histID = customer->rentalHistory.front();
                customer->rentalHistory.pop();
                if (histID != lastAction.record.movieID) {
                    newHistory.push(histID);
                }
            }
            customer->rentalHistory = newHistory;
        }
        break;
    }
    }
}
// Get the current undo stack
std::stack<RentalAction> SystemManager::getUndoStack() const {
    return undoStack;
}

// Waitlist operations
void SystemManager::addToWaitlist(int customerID, int movieID) {
    Movie* movie = getMovie(movieID);
    if (!movie) {
        throw std::runtime_error("Movie not found");
    }
    movie->waitlist.push(customerID);
}

void SystemManager::removeFromWaitlist(int customerID, int movieID) {
    Movie* movie = getMovie(movieID);
    if (!movie) {
        throw std::runtime_error("Movie not found");
    }
    std::queue<int> newQueue;
    while (!movie->waitlist.empty()) {
        int id = movie->waitlist.front();
        movie->waitlist.pop();
        if (id != customerID) {
            newQueue.push(id);
        }
    }
    movie->waitlist = newQueue;
}

std::queue<int> SystemManager::getWaitlist(int movieID) {
    Movie* movie = getMovie(movieID);
    if (!movie) {
        throw std::runtime_error("Movie not found");
    }
    return movie->waitlist;
}

// Data persistence
void SystemManager::saveToFile(const std::string& filename) {
    // Backup before saving
    std::string backupFile = filename + ".bak";
    std::remove(backupFile.c_str());
    std::rename(filename.c_str(), backupFile.c_str());
    try {
        // --- TXT Format (existing) ---
        std::ofstream file(filename);
        if (!file.is_open()) throw std::runtime_error("Failed to open file for writing: " + filename);
        // Save movies
        file << "[MOVIES]\n";
        for (const auto& [id, movie] : movies) {
            file << movie.id << ',' << movie.title << ',' << static_cast<int>(movie.genre) << ',' << movie.copiesAvailable << ',' << movie.releaseDate << ',' << movie.description << ',' << movie.coverImagePath;
            // Save waitlist
            file << ",WAITLIST:";
            std::queue<int> waitlistCopy = movie.waitlist;
            while (!waitlistCopy.empty()) {
                file << waitlistCopy.front();
                waitlistCopy.pop();
                if (!waitlistCopy.empty()) file << '|';
            }
            file << '\n';
        }
        // Save customers
        file << "[CUSTOMERS]\n";
        for (const auto& [id, customer] : customers) {
            file << customer.id << ',' << customer.name << ',' << customer.phone << ',' << customer.email << ',' << customer.role;
            // Save activeRentals
            file << ",ACTIVE:";
            std::queue<int> activeCopy = customer.activeRentals;
            while (!activeCopy.empty()) {
                file << activeCopy.front();
                activeCopy.pop();
                if (!activeCopy.empty()) file << '|';
            }
            // Save rentalHistory
            file << ",HISTORY:";
            std::queue<int> histCopy = customer.rentalHistory;
            while (!histCopy.empty()) {
                file << histCopy.front();
                histCopy.pop();
                if (!histCopy.empty()) file << '|';
            }
            file << '\n';
        }
        // Save users
        file << "[USERS]\n";
        for (const auto& [username, user] : users) {
            file << user.username << ',' << user.password << ',' << user.role << ',' << user.customerID << '\n';
        }
        // Save undo stack
        file << "[UNDO]\n";
        std::stack<RentalAction> undoCopy = undoStack;
        std::vector<RentalAction> undoVec;
        while (!undoCopy.empty()) { undoVec.push_back(undoCopy.top()); undoCopy.pop(); }
        std::reverse(undoVec.begin(), undoVec.end());
        for (const auto& action : undoVec) {
            file << (action.actionType == RentalAction::Rent ? "RENT," : "RETURN,");
            const RentalRecord& r = action.record;
            file << r.customerID << ',' << r.movieID << ',' << (long long)std::chrono::duration_cast<std::chrono::seconds>(r.rentDate.time_since_epoch()).count() << ',' << (long long)std::chrono::duration_cast<std::chrono::seconds>(r.dueDate.time_since_epoch()).count() << ',' << (long long)std::chrono::duration_cast<std::chrono::seconds>(r.returnDate.time_since_epoch()).count() << ',' << r.lateFee << '\n';
        }
        file.close();

        // --- JSON Format ---
        std::ofstream jfile("data.json");
        if (!jfile.is_open()) throw std::runtime_error("Failed to open data.json for writing");
        jfile << std::setprecision(0); // No decimals for timestamps
        jfile << "{\n";
        // Movies
        jfile << "  \"movies\": [\n";
        bool firstMovie = true;
        for (const auto& [id, movie] : movies) {
            if (!firstMovie) jfile << ",\n";
            firstMovie = false;
            jfile << "    {\n";
            jfile << "      \"id\": " << movie.id << ",\n";
            jfile << "      \"title\": \"" << movie.title << "\",\n";
            jfile << "      \"genre\": " << static_cast<int>(movie.genre) << ",\n";
            jfile << "      \"copiesAvailable\": " << movie.copiesAvailable << ",\n";
            jfile << "      \"releaseDate\": \"" << movie.releaseDate << "\",\n";
            jfile << "      \"description\": \"" << movie.description << "\",\n";
            jfile << "      \"coverImagePath\": \"" << movie.coverImagePath << "\",\n";
            // Waitlist
            jfile << "      \"waitlist\": [";
            std::queue<int> waitlistCopy = movie.waitlist;
            bool firstW = true;
            while (!waitlistCopy.empty()) {
                if (!firstW) jfile << ", ";
                firstW = false;
                jfile << waitlistCopy.front();
                waitlistCopy.pop();
            }
            jfile << "]\n    }";
        }
        jfile << "\n  ],\n";
        // Customers
        jfile << "  \"customers\": [\n";
        bool firstCust = true;
        for (const auto& [id, customer] : customers) {
            if (!firstCust) jfile << ",\n";
            firstCust = false;
            jfile << "    {\n";
            jfile << "      \"id\": " << customer.id << ",\n";
            jfile << "      \"name\": \"" << customer.name << "\",\n";
            jfile << "      \"phone\": \"" << customer.phone << "\",\n";
            jfile << "      \"email\": \"" << customer.email << "\",\n";
            jfile << "      \"role\": \"" << customer.role << "\",\n";
            // Active rentals
            jfile << "      \"activeRentals\": [";
            std::queue<int> activeCopy = customer.activeRentals;
            bool firstA = true;
            while (!activeCopy.empty()) {
                if (!firstA) jfile << ", ";
                firstA = false;
                jfile << activeCopy.front();
                activeCopy.pop();
            }
            jfile << "],\n";
            // Rental history
            jfile << "      \"rentalHistory\": [";
            std::queue<int> histCopy = customer.rentalHistory;
            bool firstH = true;
            while (!histCopy.empty()) {
                if (!firstH) jfile << ", ";
                firstH = false;
                jfile << histCopy.front();
                histCopy.pop();
            }
            jfile << "]\n    }";
        }
        jfile << "\n  ],\n";
        // Users
        jfile << "  \"users\": [\n";
        bool firstUser = true;
        for (const auto& [username, user] : users) {
            if (!firstUser) jfile << ",\n";
            firstUser = false;
            jfile << "    {\n";
            jfile << "      \"username\": \"" << user.username << "\",\n";
            jfile << "      \"password\": \"" << user.password << "\",\n";
            jfile << "      \"role\": \"" << user.role << "\",\n";
            jfile << "      \"customerID\": " << user.customerID << "\n    }";
        }
        jfile << "\n  ],\n";
        // Undo stack
        jfile << "  \"undoStack\": [\n";
        std::stack<RentalAction> undoCopyJson = undoStack;
        std::vector<RentalAction> undoVecJson;
        while (!undoCopyJson.empty()) { undoVecJson.push_back(undoCopyJson.top()); undoCopyJson.pop(); }
        std::reverse(undoVecJson.begin(), undoVecJson.end());
        bool firstU = true;
        for (const auto& action : undoVecJson) {
            if (!firstU) jfile << ",\n";
            firstU = false;
            jfile << "    {\n";
            jfile << "      \"type\": \"" << (action.actionType == RentalAction::Rent ? "RENT" : "RETURN") << "\",\n";
            const RentalRecord& r = action.record;
            jfile << "      \"customerID\": " << r.customerID << ",\n";
            jfile << "      \"movieID\": " << r.movieID << ",\n";
            jfile << "      \"rentDate\": " << (long long)std::chrono::duration_cast<std::chrono::seconds>(r.rentDate.time_since_epoch()).count() << ",\n";
            jfile << "      \"dueDate\": " << (long long)std::chrono::duration_cast<std::chrono::seconds>(r.dueDate.time_since_epoch()).count() << ",\n";
            jfile << "      \"returnDate\": " << (long long)std::chrono::duration_cast<std::chrono::seconds>(r.returnDate.time_since_epoch()).count() << ",\n";
            jfile << "      \"lateFee\": " << r.lateFee << "\n    }";
        }
        jfile << "\n  ]\n";
        jfile << "}\n";
        jfile.close();
    } catch (const std::exception& ex) {
        std::cerr << "[ERROR] Failed to save data: " << ex.what() << std::endl;
        // Try to restore backup
        std::remove(filename.c_str());
        std::rename(backupFile.c_str(), filename.c_str());
    }
}

void SystemManager::loadFromFile(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) throw std::runtime_error("Failed to open file for reading: " + filename);
        if (file.peek() == std::ifstream::traits_type::eof()) throw std::runtime_error("File is empty: " + filename);
        std::string line;
        enum Section { NONE, MOVIES, CUSTOMERS, USERS, UNDO } section = NONE;
        while (std::getline(file, line)) {
            if (line == "[MOVIES]") { section = MOVIES; continue; }
            if (line == "[CUSTOMERS]") { section = CUSTOMERS; continue; }
            if (line == "[USERS]") { section = USERS; continue; }
            if (line == "[UNDO]") { section = UNDO; continue; }
            if (line.empty()) continue;
            std::istringstream iss(line);
            if (section == MOVIES) {
                Movie m;
                std::string genreStr, waitlistStr;
                std::getline(iss, line, ','); m.id = std::stoi(line);
                std::getline(iss, m.title, ',');
                std::getline(iss, genreStr, ','); m.genre = static_cast<Genre>(std::stoi(genreStr));
                std::getline(iss, line, ','); m.copiesAvailable = std::stoi(line);
                std::getline(iss, m.releaseDate, ',');
                std::getline(iss, m.description, ',');
                std::getline(iss, m.coverImagePath, ',');
                std::getline(iss, waitlistStr, ':'); // should be "WAITLIST"
                std::getline(iss, waitlistStr);
                m.waitlist = std::queue<int>();
                if (!waitlistStr.empty()) {
                    std::istringstream wss(waitlistStr);
                    std::string idStr;
                    while (std::getline(wss, idStr, '|')) {
                        if (!idStr.empty()) m.waitlist.push(std::stoi(idStr));
                    }
                }
                movies[m.id] = m;
            } else if (section == CUSTOMERS) {
                Customer c;
                std::string activeStr, histStr;
                std::getline(iss, line, ','); c.id = std::stoi(line);
                std::getline(iss, c.name, ',');
                std::getline(iss, c.phone, ',');
                std::getline(iss, c.email, ',');
                std::getline(iss, c.role, ',');
                std::getline(iss, activeStr, ':'); // should be "ACTIVE"
                std::getline(iss, activeStr, ',');
                std::getline(iss, histStr, ':'); // should be "HISTORY"
                std::getline(iss, histStr);
                c.activeRentals = std::queue<int>();
                if (!activeStr.empty()) {
                    std::istringstream ass(activeStr);
                    std::string idStr;
                    while (std::getline(ass, idStr, '|')) {
                        if (!idStr.empty()) c.activeRentals.push(std::stoi(idStr));
                    }
                }
                c.rentalHistory = std::queue<int>();
                if (!histStr.empty()) {
                    std::istringstream hss(histStr);
                    std::string idStr;
                    while (std::getline(hss, idStr, '|')) {
                        if (!idStr.empty()) c.rentalHistory.push(std::stoi(idStr));
                    }
                }
                customers[c.id] = c;
            } else if (section == USERS) {
                User u;
                std::getline(iss, u.username, ',');
                std::getline(iss, u.password, ',');
                std::getline(iss, u.role, ',');
                std::getline(iss, line, ','); u.customerID = std::stoi(line);
                users[u.username] = u;
            } else if (section == UNDO) {
                std::string typeStr;
                std::getline(iss, typeStr, ',');
                RentalAction action;
                action.actionType = (typeStr == "RENT") ? RentalAction::Rent : RentalAction::Return;
                RentalRecord& r = action.record;
                std::getline(iss, line, ','); r.customerID = std::stoi(line);
                std::getline(iss, line, ','); r.movieID = std::stoi(line);
                std::getline(iss, line, ','); r.rentDate = std::chrono::system_clock::time_point(std::chrono::seconds(std::stoll(line)));
                std::getline(iss, line, ','); r.dueDate = std::chrono::system_clock::time_point(std::chrono::seconds(std::stoll(line)));
                std::getline(iss, line, ','); r.returnDate = std::chrono::system_clock::time_point(std::chrono::seconds(std::stoll(line)));
                std::getline(iss, line, ','); r.lateFee = std::stoi(line);
                undoStack.push(action);
            }
        }
        file.close();

        // --- JSON Format ---
        std::ifstream jfile("data.json");
        if (!jfile.is_open()) return;
        // For brevity, only load from TXT or JSON, not both. (You can extend to merge if needed.)
        // You can implement a JSON parser here if you want to load from JSON instead of TXT.
        // For now, TXT is the primary load format, JSON is for backup/debugging.
    } catch (const std::exception& ex) {
        std::cerr << "[ERROR] Failed to load data: " << ex.what() << std::endl;
        // Reinitialize with empty/default data
        movies.clear();
        customers.clear();
        users.clear();
        undoStack = std::stack<RentalAction>();
    }
}

void SystemManager::loadMoviesFromAssets() {
    std::ifstream file("movies.txt");
    if (!file.is_open()) {
        return; // File doesn't exist, skip loading
    }
    
    std::string line;
    std::string currentGenre;
    std::string currentTitle;
    std::string currentDate;
    std::string currentDescription;
    
    while (std::getline(file, line)) {
        line = line.substr(0, line.find('\r')); // Remove carriage return
        
        if (line.empty()) continue;
        
        // Check if this is a genre header
        if (line.find("🎃 Horror") != std::string::npos) {
            currentGenre = "Horror";
        } else if (line.find("🌸 Anime") != std::string::npos) {
            currentGenre = "Anime";
        } else if (line.find("💌 Romance") != std::string::npos) {
            currentGenre = "Romance";
        } else if (line.find("🔥 Action") != std::string::npos) {
            currentGenre = "Action";
        } else if (line.find("________________") != std::string::npos) {
            // End of genre section, skip
            continue;
        } else if (!currentGenre.empty()) {
            // This should be a movie title with year
            if (line.find("(") != std::string::npos && line.find(")") != std::string::npos) {
                currentTitle = line;
                // Extract year from title like "Movie Name (2023)"
                size_t start = line.find("(");
                size_t end = line.find(")");
                if (start != std::string::npos && end != std::string::npos) {
                    currentDate = line.substr(start + 1, end - start - 1);
                }
                
                // Next line should be description
                if (std::getline(file, line)) {
                    line = line.substr(0, line.find('\r'));
                    currentDescription = line;
                    
                    // Create movie object
                    Movie movie;
                    movie.id = nextMovieID++;
                    movie.title = currentTitle.substr(0, currentTitle.find("(") - 1); // Remove year from title
                    movie.genre = parseGenreFromString(currentGenre);
                    movie.copiesAvailable = 3; // Default stock of 3
                    movie.releaseDate = currentDate;
                    movie.description = currentDescription;
                    movie.coverImagePath = getCoverImagePath(movie.title, currentGenre);
                    
                    movies[movie.id] = movie;
                }
            }
        }
    }
    
    file.close();
}

void SystemManager::saveMoviesToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << "{\n";
        file << "  \"movies\": [\n";
        
        bool first = true;
        for (const auto& [id, movie] : movies) {
            if (!first) file << ",\n";
            file << "    " << SimpleJson::writeMovie(movie);
            first = false;
        }
        
        file << "\n  ]\n";
        file << "}\n";
        file.close();
    }
    
    // Also save as TXT for easy reading
    std::string txtFilename = filename.substr(0, filename.find_last_of('.')) + ".txt";
    std::ofstream txtFile(txtFilename);
    if (txtFile.is_open()) {
        txtFile << "Movie Inventory\n";
        txtFile << "==============\n\n";
        
        for (const auto& [id, movie] : movies) {
            txtFile << "ID: " << movie.id << "\n";
            txtFile << "Title: " << movie.title << "\n";
            txtFile << "Genre: " << genreToString(movie.genre) << "\n";
            txtFile << "Copies Available: " << movie.copiesAvailable << "\n";
            txtFile << "Release Date: " << movie.releaseDate << "\n";
            txtFile << "Description: " << movie.description << "\n";
            txtFile << "Cover Image: " << movie.coverImagePath << "\n";
            txtFile << "-------------------\n\n";
        }
        txtFile.close();
    }
}

void SystemManager::loadMoviesFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return;
    }
    
    try {
        std::string content((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
        file.close();
        
        // Simple JSON parsing - look for movie objects
        size_t pos = 0;
        movies.clear();
        nextMovieID = 1;
        
        while ((pos = content.find("{", pos)) != std::string::npos) {
            size_t endPos = content.find("}", pos);
            if (endPos == std::string::npos) break;
            
            std::string movieJson = content.substr(pos, endPos - pos + 1);
            Movie movie;
            if (SimpleJson::parseMovie(movieJson, movie)) {
                movies[movie.id] = movie;
                if (movie.id >= nextMovieID) {
                    nextMovieID = movie.id + 1;
                }
            }
            pos = endPos + 1;
        }
    } catch (const std::exception& e) {
        (void)e; // Silence unused variable warning
        // If JSON loading fails, try loading from assets
        loadMoviesFromAssets();
    }
}

Genre SystemManager::parseGenreFromString(const std::string& genreStr) {
    if (genreStr == "Action") return Genre::Action;
    if (genreStr == "Comedy") return Genre::Comedy;
    if (genreStr == "Drama") return Genre::Drama;
    if (genreStr == "Horror") return Genre::Action; // Map Horror to Action for now
    if (genreStr == "Anime") return Genre::Comedy; // Map Anime to Comedy for now
    if (genreStr == "Romance") return Genre::Drama; // Map Romance to Drama for now
    return Genre::Undefined;
}

std::string SystemManager::getCoverImagePath(const std::string& title, const std::string& genre) {
    std::string genreFolder;
    if (genre == "Action") genreFolder = "🔥 Action - Adventure";
    else if (genre == "Horror") genreFolder = "🎃 Horror";
    else if (genre == "Anime") genreFolder = "🌸 Anime";
    else if (genre == "Romance") genreFolder = "💌 Romance";
    else genreFolder = "🔥 Action - Adventure"; // Default
    
    std::string filename = title + ".jpg";
    return "assets/" + genreFolder + "/" + filename;
}

void SystemManager::loadUsersFromAssets() {
    // Load customers from TXT
    std::ifstream custTxt("customers.txt");
    std::string line;
    while (std::getline(custTxt, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string username, password;
        if (std::getline(iss, username, ',') && std::getline(iss, password)) {
            // Create Customer
            Customer customer;
            customer.id = nextCustomerID++;
            customer.name = username;
            customer.phone = "";
            customer.email = "";
            customer.role = "user";
            customers[customer.id] = customer;
            // Create User
            User user;
            user.username = username;
            user.password = password;
            user.role = "user";
            user.customerID = customer.id;
            users[username] = user;
        }
    }
    custTxt.close();
    // Load admins from TXT
    std::ifstream adminTxt("admins.txt");
    while (std::getline(adminTxt, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string username, password;
        if (std::getline(iss, username, ',') && std::getline(iss, password)) {
            User user;
            user.username = username;
            user.password = password;
            user.role = "admin";
            user.customerID = -1;
            users[username] = user;
        }
    }
    adminTxt.close();
    // Optionally, load from JSON as well (if present)
    std::ifstream custJson("customers.json");
    if (custJson.is_open()) {
        std::string content((std::istreambuf_iterator<char>(custJson)), std::istreambuf_iterator<char>());
        size_t pos = 0;
        while ((pos = content.find("{", pos)) != std::string::npos) {
            size_t endPos = content.find("}", pos);
            if (endPos == std::string::npos) break;
            std::string obj = content.substr(pos, endPos - pos + 1);
            std::string username, password, role;
            size_t u1 = obj.find("\"username\"");
            if (u1 != std::string::npos) {
                size_t q1 = obj.find('"', u1 + 11);
                size_t q2 = obj.find('"', q1 + 1);
                username = obj.substr(q1 + 1, q2 - q1 - 1);
            }
            size_t p1 = obj.find("\"password\"");
            if (p1 != std::string::npos) {
                size_t q1 = obj.find('"', p1 + 11);
                size_t q2 = obj.find('"', q1 + 1);
                password = obj.substr(q1 + 1, q2 - q1 - 1);
            }
            size_t r1 = obj.find("\"role\"");
            if (r1 != std::string::npos) {
                size_t q1 = obj.find('"', r1 + 6);
                size_t q2 = obj.find('"', q1 + 1);
                role = obj.substr(q1 + 1, q2 - q1 - 1);
            }
            if (!username.empty() && !password.empty() && role == "user") {
                Customer customer;
                customer.id = nextCustomerID++;
                customer.name = username;
                customer.phone = "";
                customer.email = "";
                customer.role = "user";
                customers[customer.id] = customer;
                User user;
                user.username = username;
                user.password = password;
                user.role = "user";
                user.customerID = customer.id;
                users[username] = user;
            }
            pos = endPos + 1;
        }
        custJson.close();
    }
    std::ifstream adminJson("admins.json");
    if (adminJson.is_open()) {
        std::string content((std::istreambuf_iterator<char>(adminJson)), std::istreambuf_iterator<char>());
        size_t pos = 0;
        while ((pos = content.find("{", pos)) != std::string::npos) {
            size_t endPos = content.find("}", pos);
            if (endPos == std::string::npos) break;
            std::string obj = content.substr(pos, endPos - pos + 1);
            std::string username, password, role;
            size_t u1 = obj.find("\"username\"");
            if (u1 != std::string::npos) {
                size_t q1 = obj.find('"', u1 + 11);
                size_t q2 = obj.find('"', q1 + 1);
                username = obj.substr(q1 + 1, q2 - q1 - 1);
            }
            size_t p1 = obj.find("\"password\"");
            if (p1 != std::string::npos) {
                size_t q1 = obj.find('"', p1 + 11);
                size_t q2 = obj.find('"', q1 + 1);
                password = obj.substr(q1 + 1, q2 - q1 - 1);
            }
            size_t r1 = obj.find("\"role\"");
            if (r1 != std::string::npos) {
                size_t q1 = obj.find('"', r1 + 6);
                size_t q2 = obj.find('"', q1 + 1);
                role = obj.substr(q1 + 1, q2 - q1 - 1);
            }
            if (!username.empty() && !password.empty() && role == "admin") {
                User user;
                user.username = username;
                user.password = password;
                user.role = "admin";
                user.customerID = -1;
                users[username] = user;
            }
            pos = endPos + 1;
        }
        adminJson.close();
    }
}

SystemManager::SystemManager() {
    loadFromFile("data.txt");
}
