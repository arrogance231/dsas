#include "../../include/gui/UserMenuWindow.h"
#include "../../SearchWindow.h"
#include "../../RentWindow.h"
#include "../../ReturnWindow.h"
#include "../../CurrentRentalsWindow.h"
#include "../../HistoryWindow.h" 

UserMenuWindow::UserMenuWindow(SystemManager* sysMgr)
    : wxFrame(nullptr, wxID_ANY, "User Menu", wxDefaultPosition, wxSize(400, 300)), systemManager(sysMgr)
{
    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    rentMovieBtn = new wxButton(panel, wxID_ANY, "Rent Movie");
    returnMovieBtn = new wxButton(panel, wxID_ANY, "Return A Movie");
    checkRentalsBtn = new wxButton(panel, wxID_ANY, "Check Current Rentals");
	rentalHistoryBtn = new wxButton(panel, wxID_ANY, "Rental History"); //Added rental history button
    checkInvoiceBtn = new wxButton(panel, wxID_ANY, "Check Invoice");

    sizer->Add(rentMovieBtn, 0, wxEXPAND | wxALL, 10);
    sizer->Add(returnMovieBtn, 0, wxEXPAND | wxALL, 10);
    sizer->Add(checkRentalsBtn, 0, wxEXPAND | wxALL, 10);
	sizer->Add(rentalHistoryBtn, 0, wxEXPAND | wxALL, 10);
    sizer->Add(checkInvoiceBtn, 0, wxEXPAND | wxALL, 10);

    panel->SetSizer(sizer);
    Centre();

    rentMovieBtn->Bind(wxEVT_BUTTON, &UserMenuWindow::OnRentMovie, this);
    returnMovieBtn->Bind(wxEVT_BUTTON, &UserMenuWindow::OnReturnMovie, this);
    checkRentalsBtn->Bind(wxEVT_BUTTON, &UserMenuWindow::OnCheckRentals, this);
	rentalHistoryBtn->Bind(wxEVT_BUTTON, &UserMenuWindow::OnRentalHistory, this); 
    checkInvoiceBtn->Bind(wxEVT_BUTTON, &UserMenuWindow::OnCheckInvoice, this);
}

void UserMenuWindow::OnRentMovie(wxCommandEvent& event) {
    SearchWindow* searchWin = new SearchWindow(this, *systemManager);
    if (searchWin->ShowModal() == wxID_OK) {
        int selectedMovieId = searchWin->GetSelectedMovieId();
        if (selectedMovieId != -1) {
            RentWindow* rentWin = new RentWindow(this, *systemManager, selectedMovieId);
            rentWin->ShowModal();
            rentWin->Destroy();
        }
    }
    searchWin->Destroy();
}
void UserMenuWindow::OnReturnMovie(wxCommandEvent& event) {
    User* user = systemManager->getCurrentUser();
    if (!user || user->role != "user") {
        wxMessageBox("No user is logged in.", "Error");
        return;
    }

    Customer* customer = systemManager->getCustomer(user->customerID);
    if (!customer) {
        wxMessageBox("Customer not found.", "Error");
        return;
    }

    if (customer->activeRentals.empty()) {
		wxMessageBox("You have no rentals.", "Notice"); // Stop if no rentals
        return;
    }

    ReturnWindow* returnWin = new ReturnWindow(systemManager);
    returnWin->Show();
}

void UserMenuWindow::OnCheckRentals(wxCommandEvent& event) {
    User* user = systemManager->getCurrentUser();
    if (!user || user->role != "user") {
        wxMessageBox("No user is logged in.", "Error");
        return;
    }

    Customer* customer = systemManager->getCustomer(user->customerID);
    if (!customer) {
        wxMessageBox("Customer not found.", "Error");
        return;
    }

    if (customer->activeRentals.empty()) {
		wxMessageBox("You have no rentals.", "Notice"); // Stop if no rentals
        return;
    }

    CurrentRentalsWindow* rentalsWin = new CurrentRentalsWindow(systemManager);
    rentalsWin->Show();
}

void UserMenuWindow::OnRentalHistory(wxCommandEvent& event) {
    User* user = systemManager->getCurrentUser();
    if (!user || user->role != "user") {
        wxMessageBox("No user is logged in.", "Error");
        return;
    }
    Customer* customer = systemManager->getCustomer(user->customerID);
    if (!customer) {
        wxMessageBox("Customer not found.", "Error");
        return;
    }
    if (customer->rentalHistory.empty() && customer->activeRentals.empty()) {
        wxMessageBox("You have no rental history.", "Notice"); // Stop if no rental history
        return;
    }

    HistoryWindow* historyWin = new HistoryWindow(systemManager, customer->id);
    historyWin->Show();
}

void UserMenuWindow::OnCheckInvoice(wxCommandEvent& event) {
    wxMessageBox("Check Invoice: Not implemented yet.", "Info", wxOK | wxICON_INFORMATION);
} 