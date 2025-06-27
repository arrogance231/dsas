#include "../../include/gui/LogSignWindow.h"
#include "../../SearchWindow.h"
#include "../../RentWindow.h"
#include "../../MainWindow.h"
#include "../../include/gui/UserMenuWindow.h"
LogSignWindow::LogSignWindow(wxWindow* parent, SystemManager* sysManager)
    : wxDialog(parent, wxID_ANY, "Login/Signup", wxDefaultPosition, wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
      systemManager(sysManager)
{
    // Create notebook for tabs
    notebook = new wxNotebook(this, wxID_ANY);
    // Create panels
    CreateLoginPanel();
    CreateSignupPanel();
    // Add panels to notebook
    notebook->AddPage(loginPanel, "Login");
    notebook->AddPage(signupPanel, "Sign Up");
    // Set up main sizer
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(notebook, 1, wxEXPAND | wxALL, 10);
    SetSizerAndFit(mainSizer);
    // Center window
    Centre();
    // Bind events
    loginButton->Bind(wxEVT_BUTTON, &LogSignWindow::OnLogin, this);
    signupButton->Bind(wxEVT_BUTTON, &LogSignWindow::OnSignup, this);
    this->Bind(wxEVT_CLOSE_WINDOW, &LogSignWindow::OnClose, this);
}

void LogSignWindow::CreateLoginPanel() {
    loginPanel = new wxPanel(notebook);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText* title = new wxStaticText(loginPanel, wxID_ANY, "Login to Movie Rental System");
    title->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    sizer->Add(title, 0, wxALIGN_CENTER | wxALL, 20);
    sizer->Add(new wxStaticText(loginPanel, wxID_ANY, "Username:"), 0, wxLEFT | wxRIGHT | wxTOP, 10);
    loginUsername = new wxTextCtrl(loginPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    sizer->Add(loginUsername, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    sizer->Add(new wxStaticText(loginPanel, wxID_ANY, "Password:"), 0, wxLEFT | wxRIGHT | wxTOP, 10);
    loginPassword = new wxTextCtrl(loginPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    sizer->Add(loginPassword, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    loginButton = new wxButton(loginPanel, wxID_ANY, "Login");
    sizer->Add(loginButton, 0, wxALIGN_CENTER | wxALL, 20);
    loginPanel->SetSizer(sizer);
}

void LogSignWindow::CreateSignupPanel() {
    signupPanel = new wxPanel(notebook);
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    wxStaticText* title = new wxStaticText(signupPanel, wxID_ANY, "Create New Account");
    title->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    sizer->Add(title, 0, wxALIGN_CENTER | wxALL, 20);
    sizer->Add(new wxStaticText(signupPanel, wxID_ANY, "Username:"), 0, wxLEFT | wxRIGHT | wxTOP, 10);
    signupUsername = new wxTextCtrl(signupPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    sizer->Add(signupUsername, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    sizer->Add(new wxStaticText(signupPanel, wxID_ANY, "Password:"), 0, wxLEFT | wxRIGHT | wxTOP, 10);
    signupPassword = new wxTextCtrl(signupPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    sizer->Add(signupPassword, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    sizer->Add(new wxStaticText(signupPanel, wxID_ANY, "Confirm Password:"), 0, wxLEFT | wxRIGHT | wxTOP, 10);
    signupConfirmPassword = new wxTextCtrl(signupPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    sizer->Add(signupConfirmPassword, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    sizer->Add(new wxStaticText(signupPanel, wxID_ANY, "Full Name:"), 0, wxLEFT | wxRIGHT | wxTOP, 10);
    signupName = new wxTextCtrl(signupPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    sizer->Add(signupName, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    sizer->Add(new wxStaticText(signupPanel, wxID_ANY, "Phone:"), 0, wxLEFT | wxRIGHT | wxTOP, 10);
    signupPhone = new wxTextCtrl(signupPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    sizer->Add(signupPhone, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    sizer->Add(new wxStaticText(signupPanel, wxID_ANY, "Email:"), 0, wxLEFT | wxRIGHT | wxTOP, 10);
    signupEmail = new wxTextCtrl(signupPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    sizer->Add(signupEmail, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    sizer->Add(new wxStaticText(signupPanel, wxID_ANY, "Role:"), 0, wxLEFT | wxRIGHT | wxTOP, 10);
    signupRole = new wxChoice(signupPanel, wxID_ANY);
    signupRole->Append("User");
    signupRole->Append("Admin");
    signupRole->SetSelection(0);
    sizer->Add(signupRole, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    signupButton = new wxButton(signupPanel, wxID_ANY, "Sign Up");
    sizer->Add(signupButton, 0, wxALIGN_CENTER | wxALL, 20);
    signupPanel->SetSizer(sizer);
}

void LogSignWindow::OnLogin(wxCommandEvent& event) {
    wxString username = loginUsername->GetValue();
    wxString password = loginPassword->GetValue();
    if (username.IsEmpty() || password.IsEmpty()) {
        ShowError("Please enter both username and password.");
        return;
    }
    std::string userStr = username.ToStdString();
    std::string passStr = password.ToStdString();
    if (systemManager->verifyLogin(userStr, passStr)) {
        ShowSuccess("Login successful!");
        User* user = systemManager->getCurrentUser();
        if (user && user->role == "user") {
            UserMenuWindow* userMenu = new UserMenuWindow(systemManager);
            userMenu->Show();
        } else {
            MainWindow* mainWindow = new MainWindow("Movie Rental System - Main Menu", systemManager);
            mainWindow->Show();
        }
        Destroy();
    } else {
        ShowError("Invalid username or password.");
    }
}

void LogSignWindow::OnSignup(wxCommandEvent& event) {
    if (!ValidateSignupInputs()) {
        return;
    }
    User newUser;
    newUser.username = signupUsername->GetValue().ToStdString();
    newUser.password = signupPassword->GetValue().ToStdString();
    newUser.role = (signupRole->GetSelection() == 0) ? "user" : "admin";
    if (newUser.role == "user") {
        Customer newCustomer;
        newCustomer.id = systemManager->getAllCustomers().size() + 1;
        newCustomer.name = signupName->GetValue().ToStdString();
        newCustomer.phone = signupPhone->GetValue().ToStdString();
        newCustomer.email = signupEmail->GetValue().ToStdString();
        newCustomer.role = "user";
        systemManager->addCustomer(newCustomer);
        newUser.customerID = newCustomer.id;
    } else {
        newUser.customerID = -1;
    }
    systemManager->addUser(newUser);
    ShowSuccess("Account created successfully! You can now login.");
    signupUsername->SetValue("");
    signupPassword->SetValue("");
    signupConfirmPassword->SetValue("");
    signupName->SetValue("");
    signupPhone->SetValue("");
    signupEmail->SetValue("");
    signupRole->SetSelection(0);
}

bool LogSignWindow::ValidateSignupInputs() {
    wxString username = signupUsername->GetValue();
    wxString password = signupPassword->GetValue();
    wxString confirmPassword = signupConfirmPassword->GetValue();
    wxString name = signupName->GetValue();
    wxString phone = signupPhone->GetValue();
    wxString email = signupEmail->GetValue();
    if (username.IsEmpty() || password.IsEmpty() || confirmPassword.IsEmpty() ||
        name.IsEmpty() || phone.IsEmpty() || email.IsEmpty()) {
        ShowError("Please fill in all fields.");
        return false;
    }
    if (password != confirmPassword) {
        ShowError("Passwords do not match.");
        return false;
    }
    if (password.Length() < 6) {
        ShowError("Password must be at least 6 characters long.");
        return false;
    }
    if (!email.Contains("@") || !email.Contains(".")) {
        ShowError("Please enter a valid email address.");
        return false;
    }
    return true;
}

void LogSignWindow::ShowError(const wxString& message) {
    wxMessageDialog dialog(this, message, "Error", wxOK | wxICON_ERROR);
    dialog.ShowModal();
}

void LogSignWindow::ShowSuccess(const wxString& message) {
    wxMessageDialog dialog(this, message, "Success", wxOK | wxICON_INFORMATION);
    dialog.ShowModal();
}

void LogSignWindow::OnClose(wxCloseEvent& event) {
    wxExit();
} 