#include "QueueWindow.h"

// Event IDs for controls
enum {
    ID_MOVIE_CHOICE = wxID_HIGHEST + 1,
    ID_JOIN_BUTTON,
    ID_LEAVE_BUTTON
};

// Event table to connect UI actions to functions
wxBEGIN_EVENT_TABLE(QueueWindow, wxDialog)
EVT_CHOICE(ID_MOVIE_CHOICE, QueueWindow::OnMovieSelection)
EVT_BUTTON(ID_JOIN_BUTTON, QueueWindow::OnJoinWaitlist)
EVT_BUTTON(ID_LEAVE_BUTTON, QueueWindow::OnLeaveWaitlist)
EVT_BUTTON(wxID_CLOSE, QueueWindow::OnClose)
wxEND_EVENT_TABLE()

// Constructor without preselected movie
QueueWindow::QueueWindow(wxWindow* parent, SystemManager& systemManager)
    : wxDialog(parent, wxID_ANY, "Movie Waitlist Viewer", wxDefaultPosition, wxSize(500, 400)),
    systemManager(systemManager)
{
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // ===== Movie Selector =====
    wxBoxSizer* topSizer = new wxBoxSizer(wxHORIZONTAL);
    topSizer->Add(new wxStaticText(this, wxID_ANY, "Select Movie:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);

    // Dropdown for movie selection
    movieChoice = new wxChoice(this, ID_MOVIE_CHOICE);

    // Populate movie choices from systemManager
    std::vector<Movie> movies = systemManager.getAllMovies();
    for (const Movie& movie : movies) {
        movieChoice->Append(wxString::Format("[%d] %s", movie.id, movie.title), (void*)(intptr_t)movie.id);
    }

    topSizer->Add(movieChoice, 1, wxEXPAND);
    mainSizer->Add(topSizer, 0, wxEXPAND | wxALL, 10);

    // ===== Waitlist Display =====
    waitlistBox = new wxListBox(this, wxID_ANY);
    mainSizer->Add(waitlistBox, 1, wxEXPAND | wxLEFT | wxRIGHT, 10);

    // ===== Buttons =====
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    joinButton = new wxButton(this, ID_JOIN_BUTTON, "Join Waitlist");
    leaveButton = new wxButton(this, ID_LEAVE_BUTTON, "Leave Waitlist");
    closeButton = new wxButton(this, wxID_CLOSE, "Close");

    buttonSizer->Add(joinButton, 1, wxALL, 5);
    buttonSizer->Add(leaveButton, 1, wxALL, 5);
    buttonSizer->Add(closeButton, 1, wxALL, 5);

    mainSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 10);

    // Set layout
    SetSizer(mainSizer);
}

// Constructor with a preselected movie (overloaded constructor)
QueueWindow::QueueWindow(wxWindow* parent, SystemManager& systemManager, int preselectedMovieID)
    : QueueWindow(parent, systemManager) // Calls the main constructor first
{
    // Find and select the movie based on preselectedMovieID
    for (unsigned int i = 0; i < movieChoice->GetCount(); ++i) {
        int movieID = (intptr_t)movieChoice->GetClientData(i);
        if (movieID == preselectedMovieID) {
            movieChoice->SetSelection(i);
            RefreshWaitlistDisplay();
            break;
        }
    }
}

// ===== EVENT HANDLERS =====

// When a movie is selected from dropdown
void QueueWindow::OnMovieSelection(wxCommandEvent& event) {
    RefreshWaitlistDisplay();
}

// Handle "Join Waitlist" button click
void QueueWindow::OnJoinWaitlist(wxCommandEvent& event) {
    if (movieChoice->GetSelection() == wxNOT_FOUND) {
        wxMessageBox("Please select a movie first.");
        return;
    }

    int movieID = (intptr_t)movieChoice->GetClientData(movieChoice->GetSelection());
    User* user = systemManager.getCurrentUser();

    if (!user) {
        wxMessageBox("No user logged in.");
        return;
    }

    int customerID = user->customerID;
    if (customerID == -1) {
        wxMessageBox("Admins cannot join waitlists.");
        return;
    }

    // Add user to waitlist
    systemManager.addToWaitlist(customerID, movieID);
    RefreshWaitlistDisplay();
}

// Handle "Leave Waitlist" button click
void QueueWindow::OnLeaveWaitlist(wxCommandEvent& event) {
    if (movieChoice->GetSelection() == wxNOT_FOUND) {
        wxMessageBox("Please select a movie first.");
        return;
    }

    int movieID = (intptr_t)movieChoice->GetClientData(movieChoice->GetSelection());
    User* user = systemManager.getCurrentUser();

    if (!user) {
        wxMessageBox("No user logged in.");
        return;
    }

    int customerID = user->customerID;
    if (customerID == -1) {
        wxMessageBox("Admins cannot leave waitlists.");
        return;
    }

    // Remove user from waitlist
    systemManager.removeFromWaitlist(customerID, movieID);
    RefreshWaitlistDisplay();
}

// Handle "Close" button click
void QueueWindow::OnClose(wxCommandEvent& event) {
    EndModal(wxID_CLOSE);
}

// ===== Updates the Waitlist Display =====
void QueueWindow::RefreshWaitlistDisplay() {
    waitlistBox->Clear();

    if (movieChoice->GetSelection() == wxNOT_FOUND) {
        return;
    }

    int movieID = (intptr_t)movieChoice->GetClientData(movieChoice->GetSelection());
    Movie* movie = systemManager.getMovie(movieID);

    if (!movie) return;

    // Copy the waitlist queue temporarily
    std::queue<int> tempQueue = movie->waitlist;

    while (!tempQueue.empty()) {
        int customerID = tempQueue.front();
        tempQueue.pop();

        Customer* customer = systemManager.getCustomer(customerID);
        if (customer) {
            waitlistBox->Append(wxString::Format("[%d] %s", customer->id, customer->name));
        }
        else {
            waitlistBox->Append(wxString::Format("[ID %d] Unknown Customer", customerID));
        }
    }
}
