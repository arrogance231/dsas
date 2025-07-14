// Made by Ram.
#include "CurrentRentalsWindow.h"
#include <queue>
#include <iomanip>
#include <sstream>

// Function to convert time_point to string
namespace {
    std::string FormatDate(const std::chrono::system_clock::time_point& tp) {
        std::time_t time = std::chrono::system_clock::to_time_t(tp);
        std::tm* tm_ptr = std::localtime(&time);
        std::ostringstream oss;
        oss << std::put_time(tm_ptr, "%Y-%m-%d %H:%M");
        return oss.str();
    }
}


CurrentRentalsWindow::CurrentRentalsWindow(SystemManager* sm)
    : wxFrame(nullptr, wxID_ANY, "Current Rentals", wxDefaultPosition, wxSize(900, 500)),
    systemManager(sm)
{
    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    rentalList = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxSize(880, 400),
        wxLC_REPORT | wxLC_SINGLE_SEL);

    rentalList->InsertColumn(0, "Movie ID", wxLIST_FORMAT_LEFT, 70);
    rentalList->InsertColumn(1, "Title", wxLIST_FORMAT_LEFT, 200);
    rentalList->InsertColumn(2, "Genre", wxLIST_FORMAT_LEFT, 100);
    rentalList->InsertColumn(3, "Rent Date", wxLIST_FORMAT_LEFT, 160);
    rentalList->InsertColumn(4, "Due Date", wxLIST_FORMAT_LEFT, 160);

    cancelButton = new wxButton(panel, wxID_ANY, "Close");

    sizer->Add(rentalList, 1, wxEXPAND | wxALL, 10);
    sizer->Add(cancelButton, 0, wxALIGN_CENTER | wxALL, 10);

    panel->SetSizer(sizer);

    // Bind cancel
    cancelButton->Bind(wxEVT_BUTTON, &CurrentRentalsWindow::OnCancelClicked, this);

    PopulateRentalList();
    Centre();
}

void CurrentRentalsWindow::PopulateRentalList() {
    rentalList->DeleteAllItems(); // Clear previous rentals

    User* user = systemManager->getCurrentUser();
    if (!user || user->role != "user") {
        wxMessageBox("No user is logged in.", "Error");
        Close();
        return;
    }

    Customer* customer = systemManager->getCustomer(user->customerID);
    if (!customer) return;

    std::queue<int> rentals = customer->activeRentals;
    while (!rentals.empty()) {
        int movieID = rentals.front();
        rentals.pop();

        Movie* movie = systemManager->getMovie(movieID);
        if (!movie) continue;

        // Search for the rental record in the undoStack
        std::stack<RentalAction> tempStack = systemManager->getUndoStack(); 
        std::chrono::system_clock::time_point rentDate, dueDate;
        bool found = false;

        while (!tempStack.empty()) {
            RentalAction action = tempStack.top();
            tempStack.pop();

            if (action.actionType == RentalAction::Rent &&
                action.record.customerID == customer->id &&
                action.record.movieID == movieID) {
                rentDate = action.record.rentDate;
                dueDate = action.record.dueDate;
                found = true;
                break;
            }
        }

        long index = rentalList->InsertItem(rentalList->GetItemCount(), std::to_string(movie->id));
        rentalList->SetItem(index, 1, movie->title);
        rentalList->SetItem(index, 2, genreToString(movie->genre));
        rentalList->SetItem(index, 3, found ? FormatDate(rentDate) : "Unknown");
        rentalList->SetItem(index, 4, found ? FormatDate(dueDate) : "Unknown");
    }
}
// Exit window
void CurrentRentalsWindow::OnCancelClicked(wxCommandEvent& event) {
    Close();
}
