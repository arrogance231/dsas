#include "MainWindow.h"
#include <wx/sizer.h>
#include "include/gui/CustomerWindow.h"
#include "include/gui/MovieInventoryWindow.h"
#include "include/gui/LogSignWindow.h"

// Made by Arjhine A. Ty
MainWindow::MainWindow(const wxString& title, SystemManager* sysManager) 
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(500, 400)), systemManager(sysManager) {
    
    panel = new wxPanel(this);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* label = new wxStaticText(panel, wxID_ANY, "Main Menu");
    label->SetFont(wxFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    sizer->Add(label, 0, wxALIGN_CENTER | wxALL, 20);

    customerButton = new wxButton(panel, wxID_ANY, "Manage Customers");
    movieButton = new wxButton(panel, wxID_ANY, "Manage Movie Inventory");
    logoutButton = new wxButton(panel, wxID_ANY, "Logout");

    sizer->Add(customerButton, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    sizer->Add(movieButton, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    sizer->AddStretchSpacer(1);
    sizer->Add(logoutButton, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    
    panel->SetSizer(sizer);
    
    // Bind events
    customerButton->Bind(wxEVT_BUTTON, &MainWindow::OnManageCustomers, this);
    movieButton->Bind(wxEVT_BUTTON, &MainWindow::OnManageMovies, this);
    logoutButton->Bind(wxEVT_BUTTON, &MainWindow::OnLogout, this);
}

void MainWindow::OnManageCustomers(wxCommandEvent& event) {
    CustomerWindow* customerWin = new CustomerWindow("Customer Management", systemManager);
    customerWin->Show();
}

void MainWindow::OnManageMovies(wxCommandEvent& event) {
    MovieInventoryWindow* movieWin = new MovieInventoryWindow("Movie Inventory Management", systemManager);
    movieWin->Show();
}

void MainWindow::OnLogout(wxCommandEvent& event) {
    systemManager->logout();
    LogSignWindow* loginWin = new LogSignWindow(this, systemManager); // Pass 'this' as the parent window
    loginWin->Show();
    Close();
}
