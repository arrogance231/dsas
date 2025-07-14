// Made by Ezekiel.
#include "RentWindow.h"
#include <wx/msgdlg.h>
#include "QueueWindow.h" // For QueueWindow usage
#include "ReceiptDialog.h"
#include <chrono>
#include <wx/spinctrl.h>

wxBEGIN_EVENT_TABLE(RentWindow, wxDialog)
    EVT_BUTTON(ID_RentBtn, RentWindow::OnRentMovie)
        wxEND_EVENT_TABLE()

            RentWindow::RentWindow(wxWindow *parent, SystemManager &sysMgr, int movieID)
    : wxDialog(parent, wxID_ANY, "Rent Movie", wxDefaultPosition, wxSize(500, 350)),
      systemManager(sysMgr), selectedMovieID(movieID)
{
    wxPanel *panel = new wxPanel(this);
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    // Fetch the selected movie
    Movie *movie = systemManager.getMovie(movieID);
    if (!movie)
    {
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
    wxStaticText* rentalPeriodLabel = new wxStaticText(panel, wxID_ANY, "Rental Period (days):");
    wxSpinCtrl* rentalDaysCtrl = new wxSpinCtrl(panel, wxID_ANY);
    rentalDaysCtrl->SetRange(1, 30);
    rentalDaysCtrl->SetValue(7);
    mainSizer->Add(rentalPeriodLabel, 0, wxALL, 10);
    mainSizer->Add(rentalDaysCtrl, 0, wxALL, 10);
    rentBtn = new wxButton(panel, ID_RentBtn, "Rent Movie");
    mainSizer->Add(rentBtn, 0, wxALIGN_CENTER | wxALL, 15);

    panel->SetSizer(mainSizer);
    Centre();

    // Bind rent button
    rentBtn->Bind(wxEVT_BUTTON, [=](wxCommandEvent& event) {
        // Check if user is logged in and is a customer
        User *user = systemManager.getCurrentUser();
        if (!user || user->customerID == -1)
        {
            wxMessageBox("No customer logged in", "Error", wxOK | wxICON_ERROR);
            return;
        }
        // Get the selected movie
        Movie *movie = systemManager.getMovie(selectedMovieID);
        if (!movie)
        {
            wxMessageBox("Movie not found!", "Error", wxOK | wxICON_ERROR);
            return;
        }
        // Check if copies are available
        if (movie->copiesAvailable > 0)
        {
            // Copies are available, proceed to rent
            try
            {
                int rentalDays = rentalDaysCtrl->GetValue();
                systemManager.rentMovie(user->customerID, selectedMovieID, rentalDays);
                wxMessageBox("Movie rented successfully!", "Success", wxOK | wxICON_INFORMATION);
                Close();
            }
            catch (const std::exception &e)
            {
                wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR);
            }
        }
        else
        {
            // No copies available, ask to join waitlist
            int result = wxMessageBox(
                "No copies available. Would you like to join the waitlist?",
                "Waitlist Option",
                wxYES_NO | wxICON_QUESTION);
            if (result == wxYES)
            {
                try
                {
                    systemManager.addToWaitlist(user->customerID, selectedMovieID);
                    wxMessageBox("Added to the waitlist.", "Waitlist", wxOK | wxICON_INFORMATION);
                    // Ask if the user wants to view the current waitlist
                    int view = wxMessageBox(
                        "Do you want to view the current waitlist?",
                        "View Waitlist",
                        wxYES_NO | wxICON_QUESTION);
                    if (view == wxYES)
                    {
                        QueueWindow *queueWin = new QueueWindow(this, systemManager, selectedMovieID);
                        queueWin->ShowModal();
                        queueWin->Destroy();
                    }
                }
                catch (const std::exception &e)
                {
                    wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR);
                }
                Close();
            }
        }
    });
}

void RentWindow::OnRentMovie(wxCommandEvent &event)
{
    // Check if user is logged in and is a customer
    User *user = systemManager.getCurrentUser();
    if (!user || user->customerID == -1)
    {
        wxMessageBox("No customer logged in", "Error", wxOK | wxICON_ERROR);
        return;
    }

    // Get the selected movie
    Movie *movie = systemManager.getMovie(selectedMovieID);
    if (!movie)
    {
        wxMessageBox("Movie not found!", "Error", wxOK | wxICON_ERROR);
        return;
    }

    // Check if copies are available
    if (movie->copiesAvailable > 0)
    {
        // Copies are available, proceed to rent
        try
        {
            int rentalDays = 7; // Default rental period if not specified
            systemManager.rentMovie(user->customerID, selectedMovieID, rentalDays);
            wxMessageBox("Movie rented successfully!", "Success", wxOK | wxICON_INFORMATION);
            Close();
        }
        catch (const std::exception &e)
        {
            wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR);
        }
    }
    else
    {
        // No copies available, ask to join waitlist
        int result = wxMessageBox(
            "No copies available. Would you like to join the waitlist?",
            "Waitlist Option",
            wxYES_NO | wxICON_QUESTION);

        if (result == wxYES)
        {
            try
            {
                systemManager.addToWaitlist(user->customerID, selectedMovieID);
                wxMessageBox("Added to the waitlist.", "Waitlist", wxOK | wxICON_INFORMATION);

                // Ask if the user wants to view the current waitlist
                int view = wxMessageBox(
                    "Do you want to view the current waitlist?",
                    "View Waitlist",
                    wxYES_NO | wxICON_QUESTION);

                if (view == wxYES)
                {
                    QueueWindow *queueWin = new QueueWindow(this, systemManager, selectedMovieID);
                    queueWin->ShowModal();
                    queueWin->Destroy();
                }
            }
            catch (const std::exception &e)
            {
                wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR);
            }

            Close();
        }
    }
}
