#ifndef CORE_HPP
#define CORE_HPP

#include "utils.h"
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <stack>
#include <chrono>

struct Customer {
    int id;
    std::string name;
    std::string phone;
    std::string email;

    std::queue<int> activeRentals;
    std::queue<int> rentalHistory;
    std::string role; // "admin" or "user"

};

struct Movie {
    int id;
    std::string title;
    Genre genre;
    int copiesAvailable;

    std::queue<int> waitlist; // Customer IDs
};

struct RentalRecord {
    int customerID;
    int movieID;
    std::chrono::system_clock::time_point rentDate;
    std::chrono::system_clock::time_point dueDate;
    std::chrono::system_clock::time_point returnDate;
    int lateFee = 0;
};

struct RentalAction {
    enum Type { Rent, Return } actionType;
    RentalRecord record;
};

class SystemManager {
public:
    void addCustomer(const Customer& c);
    void removeCustomer(int customerID);
    void addMovie(const Movie& m);
    void rentMovie(int customerID, int movieID);
    void returnMovie(int customerID, int movieID);
    void undoLastAction();
    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);

private:
    std::map<int, Customer> customers;
    std::map<int, Movie> movies;
    std::stack<RentalAction> undoStack;
};

#endif
