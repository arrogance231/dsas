#include "core.h"
#include <vector>
#include <chrono>
#include <ctime>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>

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
        // In a real application, you'd want a proper JSON library
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
// Get the current undo stack
std::stack<RentalAction> SystemManager::getUndoStack() const {
    return undoStack;
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

void SystemManager::loadMoviesFromAssets() {
    std::ifstream file("assets/movies.txt");
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
    std::ifstream custTxt("assets/customers.txt");
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
    std::ifstream adminTxt("assets/admins.txt");
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
    std::ifstream custJson("assets/customers.json");
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
    std::ifstream adminJson("assets/admins.json");
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
