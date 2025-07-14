// Made by Ram.
#include "../../ReturnWindow.h"
#include "ReceiptDialog.h"
#include <queue>
#include <iomanip>
#include <sstream>


 //Function to convert time_point to string
namespace {
    std::string FormatDate(const std::chrono::system_clock::time_point& tp) {
        std::time_t time = std::chrono::system_clock::to_time_t(tp);
        std::tm* tm_ptr = std::localtime(&time);
        std::ostringstream oss;
        oss << std::put_time(tm_ptr, "%Y-%m-%d %H:%M");
        return oss.str();
    }
}


ReturnWindow::ReturnWindow(SystemManager* sm)
    : wxFrame(nullptr, wxID_ANY, "Return a Movie", wxDefaultPosition, wxSize(900, 500)),
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

    returnButton = new wxButton(panel, wxID_ANY, "Return Selected Movie");
    cancelButton = new wxButton(panel, wxID_ANY, "Cancel");

    returnButton->Disable(); // Return disabled

    sizer->Add(rentalList, 1, wxEXPAND | wxALL, 10);

    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(returnButton, 0, wxALL, 5);
    buttonSizer->Add(cancelButton, 0, wxALL, 5);

    sizer->Add(buttonSizer, 0, wxALIGN_CENTER);

    panel->SetSizer(sizer);

    // Bind events
    rentalList->Bind(wxEVT_LIST_ITEM_SELECTED, &ReturnWindow::OnMovieSelected, this);
    rentalList->Bind(wxEVT_LIST_ITEM_DESELECTED, &ReturnWindow::OnMovieDeselected, this);
    returnButton->Bind(wxEVT_BUTTON, &ReturnWindow::OnReturnClicked, this);
    cancelButton->Bind(wxEVT_BUTTON, &ReturnWindow::OnCancelClicked, this);

    PopulateRentalList();
    Centre();
}

void ReturnWindow::PopulateRentalList() {
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
    if (rentalList->GetItemCount() == 0) {
        wxMessageBox("No rentals found.", "Info");
		Close(); // Close the window if no rentals are found
		return;
    }
}

void ReturnWindow::OnMovieSelected(wxListEvent& event) {
    long index = event.GetIndex();
    wxString idStr = rentalList->GetItemText(index);
    selectedMovieID = std::stoi(std::string(idStr.mb_str()));
	returnButton->Enable(); // Enable return button when an item is selected
}

void ReturnWindow::OnMovieDeselected(wxListEvent& event) {
    long selectedIndex = rentalList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    if (selectedIndex == -1) {
		returnButton->Disable(); // Disable return button if no item is selected
    }
}

void ReturnWindow::OnReturnClicked(wxCommandEvent& event) {
    User* user = systemManager->getCurrentUser();
    if (!user) return;

    int result = wxMessageBox("Are you sure you want to return this movie?", "Confirm", wxYES_NO | wxICON_QUESTION);
	if (result != wxYES) return; // User return confirmation 

    try {
        systemManager->returnMovie(user->customerID, selectedMovieID);

        Customer* customer = systemManager->getCustomer(user->customerID);
        Movie* movie = systemManager->getMovie(selectedMovieID);

        if (!movie || !customer) {
            wxMessageBox("Failed to retrieve rental details.", "Error", wxOK | wxICON_ERROR);
            return;
        }

        // Retrieve rentDate and dueDate from undo stack
        std::chrono::system_clock::time_point rentDate, dueDate;
        std::stack<RentalAction> tempStack = systemManager->getUndoStack();
        bool found = false;

        while (!tempStack.empty()) {
            RentalAction action = tempStack.top();
            tempStack.pop();

            if (action.actionType == RentalAction::Rent &&
                action.record.customerID == customer->id &&
                action.record.movieID == selectedMovieID) {
                rentDate = action.record.rentDate;
                dueDate = action.record.dueDate;
                found = true;
                break;
            }
        }

        if (!found) {
            wxMessageBox("Rental record not found.", "Error", wxOK | wxICON_ERROR);
            return;
        }

        auto now = std::chrono::system_clock::now(); // use as returnDate

        ReceiptDialog* receipt = new ReceiptDialog(this, *movie, rentDate, dueDate, &now);
        receipt->ShowModal();
        receipt->Destroy();

        PopulateRentalList();
        returnButton->Disable();

        
    }
    catch (std::exception& e) {
        wxMessageBox(e.what(), "Return Failed");
    }
}




void ReturnWindow::OnCancelClicked(wxCommandEvent& event) {
    Close();
}

