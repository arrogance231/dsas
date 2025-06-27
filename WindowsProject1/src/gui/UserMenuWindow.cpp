#include "../../include/gui/UserMenuWindow.h"
#include "../../SearchWindow.h"
#include "../../RentWindow.h"

UserMenuWindow::UserMenuWindow(SystemManager* sysMgr)
    : wxFrame(nullptr, wxID_ANY, "User Menu", wxDefaultPosition, wxSize(400, 300)), systemManager(sysMgr)
{
    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    rentMovieBtn = new wxButton(panel, wxID_ANY, "Rent Movie");
    returnMovieBtn = new wxButton(panel, wxID_ANY, "Return A Movie");
    checkRentalsBtn = new wxButton(panel, wxID_ANY, "Check Current Rentals");
    checkInvoiceBtn = new wxButton(panel, wxID_ANY, "Check Invoice");

    sizer->Add(rentMovieBtn, 0, wxEXPAND | wxALL, 10);
    sizer->Add(returnMovieBtn, 0, wxEXPAND | wxALL, 10);
    sizer->Add(checkRentalsBtn, 0, wxEXPAND | wxALL, 10);
    sizer->Add(checkInvoiceBtn, 0, wxEXPAND | wxALL, 10);

    panel->SetSizer(sizer);
    Centre();

    rentMovieBtn->Bind(wxEVT_BUTTON, &UserMenuWindow::OnRentMovie, this);
    returnMovieBtn->Bind(wxEVT_BUTTON, &UserMenuWindow::OnReturnMovie, this);
    checkRentalsBtn->Bind(wxEVT_BUTTON, &UserMenuWindow::OnCheckRentals, this);
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
    wxMessageBox("Return A Movie: Not implemented yet.", "Info", wxOK | wxICON_INFORMATION);
}
void UserMenuWindow::OnCheckRentals(wxCommandEvent& event) {
    wxMessageBox("Check Current Rentals: Not implemented yet.", "Info", wxOK | wxICON_INFORMATION);
}
void UserMenuWindow::OnCheckInvoice(wxCommandEvent& event) {
    wxMessageBox("Check Invoice: Not implemented yet.", "Info", wxOK | wxICON_INFORMATION);
} 