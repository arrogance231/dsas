
#include "HistoryWindow.h"
#include <algorithm>
#include <map>
#include <optional>
#include <sstream>
#include <iomanip>

namespace {
    std::string FormatDate(const std::chrono::system_clock::time_point& tp) {
        std::time_t time = std::chrono::system_clock::to_time_t(tp);
        std::tm* tm_ptr = std::localtime(&time);
        std::ostringstream oss;
        oss << std::put_time(tm_ptr, "%Y-%m-%d %H:%M");
        return oss.str();
    }
}

HistoryWindow::HistoryWindow(SystemManager* sysMgr, int custID)
    : wxFrame(nullptr, wxID_ANY, "Rental History", wxDefaultPosition, wxSize(900, 500)),
    systemManager(sysMgr), customerID(custID)
{
    wxPanel* panel = new wxPanel(this);
    rentalList = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxSize(880, 400),
        wxLC_REPORT | wxLC_SINGLE_SEL);

    // Columns
    rentalList->InsertColumn(0, "Movie ID", wxLIST_FORMAT_LEFT, 80);
    rentalList->InsertColumn(1, "Title", wxLIST_FORMAT_LEFT, 200);
    rentalList->InsertColumn(2, "Genre", wxLIST_FORMAT_LEFT, 100);
    rentalList->InsertColumn(3, "Rent Date", wxLIST_FORMAT_LEFT, 120);
    rentalList->InsertColumn(4, "Due Date", wxLIST_FORMAT_LEFT, 120);
    rentalList->InsertColumn(5, "Return Date", wxLIST_FORMAT_LEFT, 120);
    rentalList->InsertColumn(6, "Status", wxLIST_FORMAT_LEFT, 100);

    closeButton = new wxButton(panel, wxID_ANY, "Close");

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(rentalList, 1, wxALL | wxEXPAND, 10);
    sizer->Add(closeButton, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 10);

    panel->SetSizer(sizer);

    closeButton->Bind(wxEVT_BUTTON, &HistoryWindow::OnClose, this);

    PopulateList();
    Centre();
}

void HistoryWindow::PopulateList() {
    Customer* customer = systemManager->getCustomer(customerID);
    if (!customer) return;

    struct DisplayRental {
        int movieID;
        std::string title;
        Genre genre;
        std::chrono::system_clock::time_point rentDate;
        std::chrono::system_clock::time_point dueDate;
        std::optional<std::chrono::system_clock::time_point> returnDate;
        std::string status; // "Active" or "Completed"
    };

    // Copy and reverse undo stack
    std::vector<RentalAction> chronological;
    std::stack<RentalAction> undoCopy = systemManager->getUndoStack();
    while (!undoCopy.empty()) {
        chronological.push_back(undoCopy.top());
        undoCopy.pop();
    }
    std::reverse(chronological.begin(), chronological.end());

    // Map to merge Rent and Return into single record
    std::map<int, DisplayRental> rentals;

    for (const auto& action : chronological) {
        const RentalRecord& rec = action.record;
        if (rec.customerID != customerID)
            continue;

        Movie* movie = systemManager->getMovie(rec.movieID);
        if (!movie)
            continue;

        if (action.actionType == RentalAction::Rent) {
            rentals[rec.movieID] = {
                rec.movieID,
                movie->title,
                movie->genre,
                rec.rentDate,
                rec.dueDate,
                std::nullopt,
                "Active"
            };
        }
        else if (action.actionType == RentalAction::Return) {
            if (rentals.count(rec.movieID)) {
                rentals[rec.movieID].returnDate = rec.returnDate;
                rentals[rec.movieID].status = "Completed";
            }
        }
    }

    // Convert to vector and sort by rent date
    std::vector<DisplayRental> sortedRentals;
    for (const auto& [id, record] : rentals)
        sortedRentals.push_back(record);

    std::sort(sortedRentals.begin(), sortedRentals.end(),
        [](const DisplayRental& a, const DisplayRental& b) {
            return a.rentDate < b.rentDate;
        });

    for (const auto& r : sortedRentals) {
        long index = rentalList->InsertItem(rentalList->GetItemCount(), std::to_string(r.movieID));
        rentalList->SetItem(index, 1, r.title);
        rentalList->SetItem(index, 2, genreToString(r.genre));
        rentalList->SetItem(index, 3, wxString::FromUTF8(FormatDate(r.rentDate)));
        rentalList->SetItem(index, 4, wxString::FromUTF8(FormatDate(r.dueDate)));

        if (r.returnDate.has_value()) {
            rentalList->SetItem(index, 5, wxString::FromUTF8(FormatDate(r.returnDate.value())));
        }
        else {
            rentalList->SetItem(index, 5, "—");
        }

        rentalList->SetItem(index, 6, r.status);
    }
}

void HistoryWindow::OnClose(wxCommandEvent& event) {
    Close(true);
}
