# Guide for Rent, Return & Search Movie Windows

This guide provides detailed instructions for the developers responsible for implementing the `RentWindow`, `ReturnWindow`, and `SearchWindow`.

## Core Concepts You MUST Understand

Before you start, make sure you understand these key concepts. Your work depends on them.

### 1. The `SystemManager` is Your Brain

Your window's only job is to show information and capture user input. All the actual work—renting, returning, fetching data—is done by calling functions on the `systemManager` object. A pointer to this object is passed to your window's constructor.

### 2. Getting the Logged-In Customer is Essential

Both of your windows operate on behalf of the currently logged-in user. You **MUST** get this user first before doing anything else. The process is the same for both windows:

```cpp
// This is the most important piece of code for you.
User* currentUser = systemManager->getCurrentUser();

// ALWAYS check if the user exists and is a "user" before proceeding.
if (currentUser && currentUser->role == "user") {
    int customerId = currentUser->customerID;
    // Now you have the ID needed for rent/return operations.
} else {
    // Handle the error - maybe show a message and close the window.
    wxMessageBox("No customer is logged in!", "Error");
    Close();
    return;
}
```

---

## 📝 Task 1: Implementing the `SearchWindow`

**Assigned to:** Ezekiel

**Objective:** Create a window that allows users to search and filter movies by title, genre, or availability.

### What You Need to Do:

1. **Create the Header File (`SearchWindow.h`)**:

   - Declare a class that inherits from `wxFrame`
   - Include necessary wxWidgets headers and `core.h`
   - Add GUI controls: search text box, genre filter dropdown, availability filter, results list
   - Add event handlers for search, filter, and movie selection
   - Store a pointer to `SystemManager`

2. **Create the Implementation File (`SearchWindow.cpp`)**:

   - Constructor should take `SystemManager*` as parameter
   - Create a search interface with:
     - Text input for movie title search
     - Dropdown for genre filtering (Action, Comedy, Drama)
     - Checkbox or dropdown for availability (All, Available, Unavailable)
     - List control to show search results
     - "Search" button and "Rent Selected" button
   - Implement search logic using `systemManager->getAllMovies()`
   - Filter results based on user input
   - When a movie is selected and "Rent" is clicked, open the `RentWindow`

3. **Key Functions to Implement**:

   - `OnSearch()`: Filter movies based on search criteria
   - `OnRent()`: Get selected movie ID and open `RentWindow`
   - `PopulateResults()`: Display filtered movies in the list

4. **Integration with RentWindow**:
   - When user clicks "Rent" on a selected movie, create a new `RentWindow` instance
   - Pass the selected movie ID to the `RentWindow` constructor
   - You can close the search window or keep it open for multiple rentals

---

## 📝 Task 2: Implementing the `RentWindow`

**Assigned to:** Ezekiel

**Objective:** Create a window that allows a logged-in user to rent a movie. This window will be opened from the `SearchWindow` after a user selects a movie.

### What You Need to Do:

1. **Create the Header File (`RentWindow.h`)**:

   - Declare a class that inherits from `wxFrame`
   - Constructor should take `SystemManager*` AND the `movieID` to rent
   - Add GUI controls to display movie details and confirmation buttons
   - Store the `movieID` as a member variable

2. **Create the Implementation File (`RentWindow.cpp`)**:

   - Use the `movieID` parameter to fetch movie details with `systemManager->getMovie(movieID)`
   - Display movie title, genre, and any other relevant info
   - Create "Confirm Rent" and "Cancel" buttons
   - Implement the rent logic in your button handler

3. **Rent Logic Implementation**:

   - Get the current user using `systemManager->getCurrentUser()`
   - Check if user exists and has "user" role
   - Call `systemManager->rentMovie(customerID, movieID)`
   - Show success message and close window

4. **Error Handling**:
   - Handle cases where movie ID is invalid
   - Handle cases where no user is logged in
   - Handle cases where user is not a customer

---

## 📝 Task 3: Implementing the `ReturnWindow`

**Assigned to:** [Name of Teammate]

**Objective:** Create a window that shows the current user all their rented movies and allows them to return one.

### What You Need to Do:

1. **Create the Header File (`ReturnWindow.h`)**:

   - Declare a class that inherits from `wxFrame`
   - Constructor should take `SystemManager*` as parameter
   - Add a list control to show rented movies
   - Add a "Return Selected Movie" button
   - Store selected movie ID for return operations

2. **Create the Implementation File (`ReturnWindow.cpp`)**:

   - Create a list control with columns for Movie ID, Title, Genre
   - Create a return button (initially disabled)
   - Implement `PopulateRentalList()` function to load user's rentals

3. **Loading User's Rentals**:

   - Get current user with `systemManager->getCurrentUser()`
   - Get customer details with `systemManager->getCustomer(customerID)`
   - Access the customer's `activeRentals` queue
   - For each movie ID in the queue, get movie details and add to list
   - Store movie ID in list item data for later retrieval

4. **Return Logic Implementation**:

   - When user selects a movie from the list, enable the return button
   - When return button is clicked, get the selected movie ID
   - Call `systemManager->returnMovie(customerID, movieID)`
   - Refresh the rental list to show the movie is gone
   - Show success message

5. **UI Updates**:
   - Disable return button until a movie is selected
   - Refresh list after successful return
   - Handle empty rental lists gracefully

---

## Important Notes for All Windows:

### File Organization:

- Put header files in `WindowsProject1/include/gui/`
- Put source files in `WindowsProject1/src/gui/`
- Add both files to the project in Visual Studio

### Event Handling:

- Use `Bind()` method for event handling (modern approach)
- Always check for null pointers before using them
- Provide user feedback for all operations

### Integration:

- `SearchWindow` opens `RentWindow` when user wants to rent
- Both windows need access to the same `SystemManager` instance
- Consider how windows should behave when others are opened/closed

### Testing:

- Test with both logged-in users and no login
- Test with empty movie lists
- Test with invalid movie IDs
- Test the complete flow: Search → Rent → Return

Remember: Your windows are just the interface. Let the `SystemManager` do all the heavy lifting!
