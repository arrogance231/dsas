// Made by Sam.
#include "FeeWindow.h"
#include "utils.h"
#include <wx/spinctrl.h>

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

    // Rental days input
    sizer->Add(new wxStaticText(this, wxID_ANY, "Rental Period (days):"), 0, wxLEFT | wxRIGHT | wxTOP, 10);
    wxSpinCtrl* rentalDaysCtrl = new wxSpinCtrl(this, wxID_ANY);
    rentalDaysCtrl->SetRange(1, 30);
    rentalDaysCtrl->SetValue(7);
    sizer->Add(rentalDaysCtrl, 0, wxLEFT | wxRIGHT | wxBOTTOM, 10);

    // Rental period display
    wxStaticText* rentalPeriodText = new wxStaticText(this, wxID_ANY, "Rental Period: 7 days");
    sizer->Add(rentalPeriodText, 0, wxLEFT | wxRIGHT | wxBOTTOM, 10);

    // Fee display
    wxStaticText* pricePerDayText = new wxStaticText(this, wxID_ANY, "Price per Day: ");
    wxStaticText* baseFeeText = new wxStaticText(this, wxID_ANY, "Base Fee: ");
    wxStaticText* lateFeeText = new wxStaticText(this, wxID_ANY, "Late Fee: ");
    wxStaticText* totalText = new wxStaticText(this, wxID_ANY, "Total Fee: ");
    totalText->SetForegroundColour(*wxRED);
    sizer->Add(pricePerDayText, 0, wxLEFT | wxRIGHT | wxBOTTOM, 10);
    sizer->Add(baseFeeText, 0, wxLEFT | wxRIGHT | wxBOTTOM, 10);
    sizer->Add(lateFeeText, 0, wxLEFT | wxRIGHT | wxBOTTOM, 10);
    sizer->Add(totalText, 0, wxLEFT | wxRIGHT | wxBOTTOM, 15);

    // Calculation lambda
    auto updateFees = [=]() {
        int pricePerDay = getPricePerDay(movie->genre);
        int rentalDays = rentalDaysCtrl->GetValue();
        int baseFee = pricePerDay * rentalDays;
        // Simulate a late return for demo (0 late days)
        int lateFee = 0;
        int total = baseFee + lateFee;
        pricePerDayText->SetLabel("Price per Day: " + std::to_string(pricePerDay));
        baseFeeText->SetLabel("Base Fee: " + std::to_string(pricePerDay) + " x " + std::to_string(rentalDays) + ": " + std::to_string(baseFee));
        lateFeeText->SetLabel("Late Fee: " + std::to_string(lateFee));
        totalText->SetLabel("Total Fee: " + std::to_string(total));
        rentalPeriodText->SetLabel("Rental Period: " + std::to_string(rentalDays) + " days");
    };
    updateFees();
    rentalDaysCtrl->Bind(wxEVT_SPINCTRL, [=](wxCommandEvent&) { updateFees(); });

    // OK Button
    wxButton* okButton = new wxButton(this, wxID_OK, "OK");
    sizer->Add(okButton, 0, wxALIGN_CENTER | wxALL, 10);

    SetSizerAndFit(sizer);
}
