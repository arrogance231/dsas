#include "OverdueWindow.h"
#include "utils.h" // For isOverdue() function
#include <sstream>
#include <iomanip>

// Convert time_point to string
static std::string FormatDate(const std::chrono::system_clock::time_point& tp) {
    std::time_t time = std::chrono::system_clock::to_time_t(tp);
    std::tm* tm_ptr = std::localtime(&time);
    std::ostringstream oss;
    oss << std::put_time(tm_ptr, "%Y-%m-%d %H:%M");
    return oss.str();
}

// Constructor for viewing overdue rentals for a SPECIFIC movie
OverdueWindow::OverdueWindow(wxWindow* parent, SystemManager& systemManager, int movieID)
    : wxDialog(parent, wxID_ANY, "Overdue Rentals for Movie", wxDefaultPosition, wxSize(600, 400)),
    systemManager(systemManager) // Store system reference
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Create the grid
    grid = new wxGrid(this, wxID_ANY);
    grid->CreateGrid(0, 4);

    // Set column labels
    grid->SetColLabelValue(0, "Customer");
    grid->SetColLabelValue(1, "Movie");
    grid->SetColLabelValue(2, "Due Date");
    grid->SetColLabelValue(3, "Status");

    // Populate with rentals for this movie
    PopulateGridForMovie(movieID);

    mainSizer->Add(grid, 1, wxEXPAND | wxALL, 10);

    // Add Close button
    wxButton* closeButton = new wxButton(this, wxID_OK, "Close");
    mainSizer->Add(closeButton, 0, wxALIGN_CENTER | wxBOTTOM, 10);

    SetSizer(mainSizer);
    Centre();
}

// Populate grid for a SPECIFIC movie rental
void OverdueWindow::PopulateGridForMovie(int movieID) {
    // Clear previous grid data
    grid->ClearGrid();
    if (grid->GetNumberRows() > 0) {
        grid->DeleteRows(0, grid->GetNumberRows());
    }

    // Get movie object
    Movie* movie = systemManager.getMovie(movieID);
    if (!movie) return;

    // Get all customers
    auto customers = systemManager.getAllCustomers();

    // Loop through customers
    for (const auto& customer : customers) {
        std::queue<int> rentals = customer.activeRentals;
        while (!rentals.empty()) {
            int rentedMovieID = rentals.front();
            rentals.pop();
            if (rentedMovieID != movieID) continue;
            // Find the rental record for this customer and movie
            std::stack<RentalAction> tempStack = systemManager.getUndoStack();
            std::chrono::system_clock::time_point dueTime;
            bool found = false;
            while (!tempStack.empty()) {
                RentalAction action = tempStack.top();
                tempStack.pop();
                if (action.actionType == RentalAction::Rent &&
                    action.record.customerID == customer.id &&
                    action.record.movieID == movieID) {
                    dueTime = action.record.dueDate;
                    found = true;
                    break;
                }
            }
            if (!found) continue;
            bool overdue = isOverdue(dueTime);
            wxString dueDateStr = wxString(FormatDate(dueTime));
            wxString status = overdue ? "Overdue" : "On Time";
            int row = grid->GetNumberRows();
            grid->AppendRows(1);
            grid->SetCellValue(row, 0, customer.name);
            grid->SetCellValue(row, 1, movie->title);
            grid->SetCellValue(row, 2, dueDateStr);
            grid->SetCellValue(row, 3, status);
            if (overdue) {
                grid->SetCellTextColour(row, 3, *wxRED);
                grid->SetCellBackgroundColour(row, 3, wxColour(255, 228, 225));
            }
        }
    }
}
