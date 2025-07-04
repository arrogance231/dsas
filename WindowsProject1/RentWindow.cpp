#include "RentWindow.h"
#include <wx/msgdlg.h>
#include "ReceiptDialog.h"
#include <chrono>

wxBEGIN_EVENT_TABLE(RentWindow, wxDialog)
    EVT_BUTTON(ID_RentBtn, RentWindow::OnRentMovie)
wxEND_EVENT_TABLE()

RentWindow::RentWindow(wxWindow* parent, SystemManager& sysMgr, int movieID)
    : wxDialog(parent, wxID_ANY, "Rent Movie", wxDefaultPosition, wxSize(500, 350)),
      systemManager(sysMgr), selectedMovieID(movieID)
{
    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Fetch the selected movie
    Movie* movie = systemManager.getMovie(movieID);
    if (!movie) {
        wxMessageBox("Movie not found!", "Error", wxOK | wxICON_ERROR);
        Close();
        return;
    }

    // Movie details
    movieTitleText = new wxStaticText(panel, wxID_ANY, "Title: " + wxString(movie->title));
    movieGenreText = new wxStaticText(panel, wxID_ANY, "Genre: " + wxString::FromUTF8(genreToString(movie->genre)));
    movieReleaseDateText = new wxStaticText(panel, wxID_ANY, "Release Date: " + wxString(movie->releaseDate));
    movieDescriptionText = new wxStaticText(panel, wxID_ANY, "Description: " + wxString(movie->description));

    mainSizer->Add(movieTitleText, 0, wxALL, 10);
    mainSizer->Add(movieGenreText, 0, wxALL, 10);
    mainSizer->Add(movieReleaseDateText, 0, wxALL, 10);
    mainSizer->Add(movieDescriptionText, 0, wxALL, 10);

    // Rent button
    rentBtn = new wxButton(panel, ID_RentBtn, "Rent Movie");
    mainSizer->Add(rentBtn, 0, wxALIGN_CENTER | wxALL, 15);

    panel->SetSizer(mainSizer);
    Centre();

    // Bind rent button
    rentBtn->Bind(wxEVT_BUTTON, &RentWindow::OnRentMovie, this);
}

void RentWindow::OnRentMovie(wxCommandEvent& event) {
    User* user = systemManager.getCurrentUser();
    if (!user || user->customerID == -1) {
        wxMessageBox("No customer logged in", "Error", wxOK | wxICON_ERROR);
        return;
    }
    try {
        systemManager.rentMovie(user->customerID, selectedMovieID);
        wxMessageBox("Movie rented successfully!", "Success", wxOK | wxICON_INFORMATION);

        // Get the movie and rental info
        Movie* movie = systemManager.getMovie(selectedMovieID);
        if (!movie) return;

        // Traverse undo stack to get rent & due dates
        std::stack<RentalAction> tempStack = systemManager.getUndoStack();
        std::chrono::system_clock::time_point rentDate, dueDate;
        bool found = false;

        while (!tempStack.empty()) {
            RentalAction action = tempStack.top();
            tempStack.pop();

            if (action.actionType == RentalAction::Rent &&
                action.record.customerID == user->customerID &&
                action.record.movieID == selectedMovieID) {
                rentDate = action.record.rentDate;
                dueDate = action.record.dueDate;
                found = true;
                break;
            }
        }

        if (found) {
            // Show receipt dialog with no return date
            ReceiptDialog* receipt = new ReceiptDialog(this, *movie, rentDate, dueDate, nullptr);
            receipt->ShowModal();
            receipt->Destroy();
        }

        Close();
    } catch (const std::exception& e) {
        wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR);
    }
}
