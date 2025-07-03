#include "FeeWindow.h"
#include "utils.h"

FeeWindow::FeeWindow(wxWindow* parent, SystemManager& systemManager, int movieID)
    : wxDialog(parent, wxID_ANY, "Rental Fee", wxDefaultPosition, wxSize(350, 250)) {
    calculateAndShowFee(systemManager, movieID);
}

void FeeWindow::calculateAndShowFee(SystemManager& systemManager, int movieID) {
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    Movie* movie = systemManager.getMovie(movieID);
    if (!movie) {
        wxMessageBox("Movie not found", "Error", wxOK | wxICON_ERROR);
        return;
    }

    // Placeholder rental record (should be real in production)
    RentalRecord record;
    record.movieID = movieID;
    record.customerID = systemManager.getCurrentUser()->customerID;
    record.rentDate = std::chrono::system_clock::now() - std::chrono::hours(240); // 10 days ago
    record.dueDate = record.rentDate + std::chrono::hours(168); // 7-day rental
    record.returnDate = std::chrono::system_clock::now(); // Returned now

    int pricePerDay = getPricePerDay(movie->genre);
    int rentalDays = 7;
    int baseFee = pricePerDay * rentalDays;
    int lateFee = calculateLateFee(record.dueDate, record.returnDate);
    int total = baseFee + lateFee;

    // --- Add Labels and Values ---
    sizer->Add(new wxStaticText(this, wxID_ANY, "Movie Title: " + movie->title), 0, wxALL, 10);
    sizer->Add(new wxStaticText(this, wxID_ANY, "Genre: " + genreToString(movie->genre)), 0, wxLEFT | wxRIGHT | wxBOTTOM, 10);
    sizer->Add(new wxStaticText(this, wxID_ANY, "Rental Period: 7 days"), 0, wxLEFT | wxRIGHT | wxBOTTOM, 10);
    sizer->Add(new wxStaticText(this, wxID_ANY, "Price per Day: " + std::to_string(pricePerDay)), 0, wxLEFT | wxRIGHT | wxBOTTOM, 10);
    sizer->Add(new wxStaticText(this, wxID_ANY, "Base Fee :" + std::to_string(pricePerDay) + " x 7): " + std::to_string(baseFee)), 0, wxLEFT | wxRIGHT | wxBOTTOM, 10);
    sizer->Add(new wxStaticText(this, wxID_ANY, "Late Fee: " + std::to_string(lateFee)), 0, wxLEFT | wxRIGHT | wxBOTTOM, 10);

    // Highlighted total
    wxStaticText* totalText = new wxStaticText(this, wxID_ANY, "Total Fee: " + std::to_string(total));
    totalText->SetForegroundColour(*wxRED);
    sizer->Add(totalText, 0, wxLEFT | wxRIGHT | wxBOTTOM, 15);

    // OK Button
    wxButton* okButton = new wxButton(this, wxID_OK, "OK");
    sizer->Add(okButton, 0, wxALIGN_CENTER | wxALL, 10);

    SetSizerAndFit(sizer);
}
