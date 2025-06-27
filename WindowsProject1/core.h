#ifndef CORE_HPP
#define CORE_HPP

#include "utils.h"
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <stack>
#include <chrono>

struct User {
    std::string username;
    std::string password;
    std::string role; // "admin" or "user"
    int customerID; // -1 if admin, actual ID if user
};

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
    std::string releaseDate;
    std::string description;
    std::string coverImagePath;
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
    // User management
    void addUser(const User& user);
    bool verifyLogin(const std::string& username, const std::string& password);
    User* getCurrentUser();
    void logout();
    
    // Customer management
    void addCustomer(const Customer& c);
    void editCustomer(int customerID, const Customer& c);
    void removeCustomer(int customerID);
    Customer* getCustomer(int customerID);
    std::vector<Customer> getAllCustomers();
    
    // Movie management
    void addMovie(const Movie& m);
    void editMovie(int movieID, const Movie& m);
    void removeMovie(int movieID);
    Movie* getMovie(int movieID);
    std::vector<Movie> getAllMovies();
    void loadMoviesFromAssets();
    void saveMoviesToFile(const std::string& filename);
    void loadMoviesFromFile(const std::string& filename);
    
    // Rental operations
    void rentMovie(int customerID, int movieID);
    void returnMovie(int customerID, int movieID);
    void undoLastAction();
    
    // Waitlist operations
    void addToWaitlist(int customerID, int movieID);
    void removeFromWaitlist(int customerID, int movieID);
    std::queue<int> getWaitlist(int movieID);
    
    // Data persistence
    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);

    void loadUsersFromAssets();

private:
    std::map<std::string, User> users;
    std::map<int, Customer> customers;
    std::map<int, Movie> movies;
    std::stack<RentalAction> undoStack;
    User* currentUser = nullptr;
    int nextCustomerID = 1;
    int nextMovieID = 1;
    
    // Helper methods for movie loading
    Genre parseGenreFromString(const std::string& genreStr);
    std::string getCoverImagePath(const std::string& title, const std::string& genre);
};

#endif
