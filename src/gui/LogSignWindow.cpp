#include "include/gui/LogSignWindow.h"
#include "SearchWindow.h"
#include "RentWindow.h"
#include "MainWindow.h"

LogSignWindow::LogSignWindow(wxWindow* parent, SystemManager* systemManager)
    : wxDialog(parent, wxID_ANY, "Login/Signup", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
      systemManager(systemManager)
{
    // Create notebook for tabs
    this->notebook = new wxNotebook(this, wxID_ANY);
    // Create panels
    this->CreateLoginPanel();
    this->CreateSignupPanel();
    // Add panels to notebook
    this->notebook->AddPage(this->loginPanel, "Login");
    this->notebook->AddPage(this->signupPanel, "Sign Up");
    // Set up main sizer
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(this->notebook, 1, wxEXPAND | wxALL, 10);
    this->SetSizerAndFit(mainSizer);
    // Center window
    this->Centre();
    // Bind events
    this->loginButton->Bind(wxEVT_BUTTON, &LogSignWindow::OnLogin, this);
    this->signupButton->Bind(wxEVT_BUTTON, &LogSignWindow::OnSignup, this);
    this->Bind(wxEVT_CLOSE_WINDOW, &LogSignWindow::OnClose, this);
}

void LogSignWindow::OnLogin(wxCommandEvent& event)
{
    // ... existing code ...

    if (systemManager->verifyLogin(userStr, passStr)) {
        ShowSuccess("Login successful!");
        User* user = systemManager->getCurrentUser();
        if (user && user->role == "user") {
            // Show SearchWindow for users
            SearchWindow* searchWin = new SearchWindow(nullptr, *systemManager);
            if (searchWin->ShowModal() == wxID_OK) {
                int selectedMovieId = searchWin->GetSelectedMovieId();
                if (selectedMovieId != -1) {
                    // Open RentWindow for the selected movie
                    RentWindow* rentWin = new RentWindow(nullptr, *systemManager);
                    rentWin->Show();
                }
            }
            searchWin->Destroy();
        } else {
            // Show MainWindow for admins
            MainWindow* mainWindow = new MainWindow("Movie Rental System - Main Menu", systemManager);
            mainWindow->Show();
        }
        Destroy();
    } else {
        // ... existing code ...
    }
}

void LogSignWindow::OnSignup(wxCommandEvent& event)
{
    // ... existing code ...
}

void LogSignWindow::ShowSuccess(const wxString& message)
{
    // ... existing code ...
}

void LogSignWindow::ShowError(const wxString& message)
{
    // ... existing code ...
} 