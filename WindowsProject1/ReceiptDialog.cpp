
#include "ReceiptDialog.h"
#include "utils.h"
#include <sstream>
#include <iomanip>

// Convert time_point to string
namespace {
    std::string FormatDate(const std::chrono::system_clock::time_point& tp) {
        std::time_t time = std::chrono::system_clock::to_time_t(tp);
        std::tm* tm_ptr = std::localtime(&time);
        std::ostringstream oss;
        oss << std::put_time(tm_ptr, "%Y-%m-%d %H:%M");
        return oss.str();
    }
}

ReceiptDialog::ReceiptDialog(wxWindow* parent,
    const Movie& movie,
    const std::chrono::system_clock::time_point& rentDate,
    const std::chrono::system_clock::time_point& dueDate,
    const std::chrono::system_clock::time_point* returnDate)
    : wxDialog(parent, wxID_ANY, "Receipt", wxDefaultPosition, wxSize(400, 300))
{
    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    wxStaticText* titleText = new wxStaticText(panel, wxID_ANY, "Movie Title: " + movie.title);
    wxStaticText* genreText = new wxStaticText(panel, wxID_ANY, "Genre: " + genreToString(movie.genre));
    wxStaticText* rentDateText = new wxStaticText(panel, wxID_ANY, "Rent Date: " + FormatDate(rentDate));
    wxStaticText* dueDateText = new wxStaticText(panel, wxID_ANY, "Due Date: " + FormatDate(dueDate));
    wxStaticText* returnDateText = new wxStaticText(panel, wxID_ANY,
        "Return Date: " + (returnDate ? FormatDate(*returnDate) : "N/A"));

	int baseFee = getPricePerDay(movie.genre); //using utils function to be implemented
    wxStaticText* rentalFeeText = new wxStaticText(panel, wxID_ANY, "Rental Fee: Php" + std::to_string(baseFee));


    mainSizer->Add(titleText, 0, wxALL, 10);
    mainSizer->Add(genreText, 0, wxALL, 10);
    mainSizer->Add(rentDateText, 0, wxALL, 10);
    mainSizer->Add(dueDateText, 0, wxALL, 10);
    mainSizer->Add(returnDateText, 0, wxALL, 10);
	mainSizer->Add(rentalFeeText, 0, wxALL, 10);

    if (returnDate && *returnDate > dueDate) {
		int lateFee = calculateLateFee(dueDate, *returnDate); // using utils function to be implemented
        wxStaticText* lateFeeText = new wxStaticText(panel, wxID_ANY, "Late Fee: Php" + std::to_string(lateFee));
        mainSizer->Add(lateFeeText, 0, wxALL, 10);
    }

    closeButton = new wxButton(panel, wxID_ANY, "Close");
    mainSizer->Add(closeButton, 0, wxALIGN_CENTER | wxALL, 15);

    closeButton->Bind(wxEVT_BUTTON, &ReceiptDialog::OnClose, this);

    panel->SetSizer(mainSizer);
    Centre();
}

void ReceiptDialog::OnClose(wxCommandEvent& event) {
    Close();
}
