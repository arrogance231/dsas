#include "../../include/gui/LogSignWindow.h"
#include "../../MainWindow.h"

LogSignWindow::LogSignWindow(const wxString& title, SystemManager* sysManager)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxDefaultSize),
      systemManager(sysManager) {
    
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
    
    // Title
    wxStaticText* title = new wxStaticText(loginPanel, wxID_ANY, "Login to Movie Rental System");
    title->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    sizer->Add(title, 0, wxALIGN_CENTER | wxALL, 20);
    
    // Username
    sizer->Add(new wxStaticText(loginPanel, wxID_ANY, "Username:"), 0, wxLEFT | wxRIGHT | wxTOP, 10);
    loginUsername = new wxTextCtrl(loginPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    sizer->Add(loginUsername, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    
    // Password
    sizer->Add(new wxStaticText(loginPanel, wxID_ANY, "Password:"), 0, wxLEFT | wxRIGHT | wxTOP, 10);
    loginPassword = new wxTextCtrl(loginPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    sizer->Add(loginPassword, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    
    // Login button
    loginButton = new wxButton(loginPanel, wxID_ANY, "Login");
    sizer->Add(loginButton, 0, wxALIGN_CENTER | wxALL, 20);
    
    loginPanel->SetSizer(sizer);
}

void LogSignWindow::CreateSignupPanel() {
    signupPanel = new wxPanel(notebook);
    
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    
    // Title
    wxStaticText* title = new wxStaticText(signupPanel, wxID_ANY, "Create New Account");
    title->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
    sizer->Add(title, 0, wxALIGN_CENTER | wxALL, 20);
    
    // Username
    sizer->Add(new wxStaticText(signupPanel, wxID_ANY, "Username:"), 0, wxLEFT | wxRIGHT | wxTOP, 10);
    signupUsername = new wxTextCtrl(signupPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    sizer->Add(signupUsername, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    
    // Password
    sizer->Add(new wxStaticText(signupPanel, wxID_ANY, "Password:"), 0, wxLEFT | wxRIGHT | wxTOP, 10);
    signupPassword = new wxTextCtrl(signupPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    sizer->Add(signupPassword, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    
    // Confirm Password
    sizer->Add(new wxStaticText(signupPanel, wxID_ANY, "Confirm Password:"), 0, wxLEFT | wxRIGHT | wxTOP, 10);
    signupConfirmPassword = new wxTextCtrl(signupPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    sizer->Add(signupConfirmPassword, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    
    // Name
    sizer->Add(new wxStaticText(signupPanel, wxID_ANY, "Full Name:"), 0, wxLEFT | wxRIGHT | wxTOP, 10);
    signupName = new wxTextCtrl(signupPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    sizer->Add(signupName, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    
    // Phone
    sizer->Add(new wxStaticText(signupPanel, wxID_ANY, "Phone:"), 0, wxLEFT | wxRIGHT | wxTOP, 10);
    signupPhone = new wxTextCtrl(signupPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    sizer->Add(signupPhone, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    
    // Email
    sizer->Add(new wxStaticText(signupPanel, wxID_ANY, "Email:"), 0, wxLEFT | wxRIGHT | wxTOP, 10);
    signupEmail = new wxTextCtrl(signupPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize);
    sizer->Add(signupEmail, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    
    // Role selection
    sizer->Add(new wxStaticText(signupPanel, wxID_ANY, "Role:"), 0, wxLEFT | wxRIGHT | wxTOP, 10);
    signupRole = new wxChoice(signupPanel, wxID_ANY);
    signupRole->Append("User");
    signupRole->Append("Admin");
    signupRole->SetSelection(0); // Default to User
    sizer->Add(signupRole, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    
    // Signup button
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
    
    // Convert to std::string and verify login
    std::string userStr = username.ToStdString();
    std::string passStr = password.ToStdString();
    
    if (systemManager->verifyLogin(userStr, passStr)) {
        ShowSuccess("Login successful!");
        
        // Open main window and pass the system manager
        MainWindow* mainWindow = new MainWindow("Movie Rental System - Main Menu", systemManager);
        mainWindow->Show();
        
        // Destroy the login window instead of closing it to prevent app exit
        Destroy();
    } else {
        ShowError("Invalid username or password.");
    }
}

void LogSignWindow::OnSignup(wxCommandEvent& event) {
    if (!ValidateSignupInputs()) {
        return;
    }
    
    // Create new user
    User newUser;
    newUser.username = signupUsername->GetValue().ToStdString();
    newUser.password = signupPassword->GetValue().ToStdString();
    newUser.role = (signupRole->GetSelection() == 0) ? "user" : "admin";
    
    // Create customer if user role
    if (newUser.role == "user") {
        Customer newCustomer;
        newCustomer.id = systemManager->getAllCustomers().size() + 1; // Simple ID generation
        newCustomer.name = signupName->GetValue().ToStdString();
        newCustomer.phone = signupPhone->GetValue().ToStdString();
        newCustomer.email = signupEmail->GetValue().ToStdString();
        newCustomer.role = "user";
        
        systemManager->addCustomer(newCustomer);
        newUser.customerID = newCustomer.id;
    } else {
        newUser.customerID = -1; // Admin
    }
    
    systemManager->addUser(newUser);
    ShowSuccess("Account created successfully! You can now login.");
    
    // Clear form
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
    
    // Basic email validation
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
    // Exit application when login window is closed
    wxExit();
} 