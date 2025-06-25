#include "core.h"
#include <vector>
#include <chrono>
#include <ctime>
#include <stdexcept>

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
// Rental operations
void SystemManager::rentMovie(int customerID, int movieID) {
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
    record.dueDate = record.rentDate + std::chrono::hours(72); // 3 days from now

    // Add to undo stack
    RentalAction action;
    action.actionType = RentalAction::Rent;
    action.record = record;
    undoStack.push(action);

    // Update system state
    movie->copiesAvailable--;
    customer->activeRentals.push(movieID);
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

    // Create return record
    RentalRecord record;
    record.customerID = customerID;
    record.movieID = movieID;
    record.returnDate = std::chrono::system_clock::now();

    // Simplified due date calculation (in real system, would lookup actual due date)
    record.dueDate = record.returnDate - std::chrono::hours(72);

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
