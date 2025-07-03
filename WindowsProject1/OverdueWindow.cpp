#include "OverdueWindow.h"
#include "utils.h" // For isOverdue() function



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

        // Loop through each rental
        while (!rentals.empty()) {
            int rentedMovieID = rentals.front();
            rentals.pop();

            // Only process if movie matches
            if (rentedMovieID != movieID) continue;

            // Simulate rent and due time (PLACEHOLDER ONLY)
            auto rentTime = std::chrono::system_clock::now() - std::chrono::hours();
            auto dueTime = rentTime + std::chrono::hours(168);

            // Check if overdue
            bool overdue = isOverdue(dueTime);

            // Due date string (placeholder)
            wxString dueDateStr = wxDateTime::Now().FormatISODate();
            wxString status = overdue ? "Overdue" : "On Time";

            // Add new row
            int row = grid->GetNumberRows();
            grid->AppendRows(1);

            // Fill cells
            grid->SetCellValue(row, 0, customer.name);
            grid->SetCellValue(row, 1, movie->title);
            grid->SetCellValue(row, 2, dueDateStr);
            grid->SetCellValue(row, 3, status);

            // Color overdue rows
            if (overdue) {
                grid->SetCellTextColour(row, 3, *wxRED);
                grid->SetCellBackgroundColour(row, 3, wxColour(255, 228, 225));
            }
        }
    }
}
