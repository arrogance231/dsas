#include "core.h"
#include <vector>

// User management
void SystemManager::addUser(const User& user) {
    users[user.username] = user;
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
}

void SystemManager::editCustomer(int customerID, const Customer& c) {
    // Find the customer by their ID and update their record.
    if (customers.count(customerID)) {
        customers[customerID] = c;
    }
}

void SystemManager::removeCustomer(int customerID) {
    customers.erase(customerID);
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
}

void SystemManager::editMovie(int movieID, const Movie& m) {
    // Find the movie by its ID and update its record.
    if (movies.count(movieID)) {
        movies[movieID] = m;
    }
}

void SystemManager::removeMovie(int movieID) {
    movies.erase(movieID);
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
void SystemManager::rentMovie(int customerID, int movieID) {
    // TODO: implement
}

void SystemManager::returnMovie(int customerID, int movieID) {
    // TODO: implement
}

void SystemManager::undoLastAction() {
    // TODO: implement
}

// Waitlist operations
void SystemManager::addToWaitlist(int customerID, int movieID) {
    // Implementation needed
}

void SystemManager::removeFromWaitlist(int customerID, int movieID) {
    // Implementation needed
}

std::queue<int> SystemManager::getWaitlist(int movieID) {
    // Implementation needed
    return std::queue<int>();
}

// Data persistence
void SystemManager::saveToFile(const std::string& filename) {
    // TODO: implement
}

void SystemManager::loadFromFile(const std::string& filename) {
    // TODO: implement
}
