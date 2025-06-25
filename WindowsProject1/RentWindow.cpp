#include "RentWindow.h"
#include "SearchWindow.h"
#include <wx/msgdlg.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/grid.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <iomanip>
#include <sstream>

wxBEGIN_EVENT_TABLE(RentWindow, wxFrame)
EVT_BUTTON(ID_RentBtn, RentWindow::OnRentMovie)
EVT_BUTTON(ID_ReturnBtn, RentWindow::OnReturnMovie)
EVT_BUTTON(ID_SearchBtn, RentWindow::OnSearchMovie)
EVT_BUTTON(ID_UndoBtn, RentWindow::OnUndoAction)
wxEND_EVENT_TABLE()

RentWindow::RentWindow(wxWindow* parent, SystemManager& sysMgr)
    : wxFrame(parent, wxID_ANY, "Movie Rental System", wxDefaultPosition, wxSize(900, 650)),
    systemManager(sysMgr) {

    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Movie ID input
    wxBoxSizer* inputSizer = new wxBoxSizer(wxHORIZONTAL);
    inputSizer->Add(new wxStaticText(panel, wxID_ANY, "Movie ID:"), 0, wxALIGN_CENTER_VERTICAL | wxRIGHT, 10);
    movieIdCtrl = new wxTextCtrl(panel, wxID_ANY);
    inputSizer->Add(movieIdCtrl, 1, wxEXPAND);

    searchBtn = new wxButton(panel, ID_SearchBtn, "Search...");
    inputSizer->Add(searchBtn, 0, wxLEFT, 10);
    mainSizer->Add(inputSizer, 0, wxEXPAND | wxALL, 10);

    // Buttons
    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
    rentBtn = new wxButton(panel, ID_RentBtn, "Rent Movie");
    returnBtn = new wxButton(panel, ID_ReturnBtn, "Return Movie");
    undoBtn = new wxButton(panel, ID_UndoBtn, "Undo Last Action");

    btnSizer->Add(rentBtn, 0, wxRIGHT, 10);
    btnSizer->Add(returnBtn, 0, wxRIGHT, 10);
    btnSizer->Add(undoBtn, 0);
    mainSizer->Add(btnSizer, 0, wxALIGN_CENTER | wxBOTTOM, 10);

    // Rentals grid
    rentalsGrid = new wxGrid(panel, wxID_ANY);
    rentalsGrid->CreateGrid(0, 4);
    rentalsGrid->SetColLabelValue(0, "Movie ID");
    rentalsGrid->SetColLabelValue(1, "Title");
    rentalsGrid->SetColLabelValue(2, "Rent Date");
    rentalsGrid->SetColLabelValue(3, "Due Date");
    rentalsGrid->AutoSizeColumns();
    mainSizer->Add(rentalsGrid, 1, wxEXPAND | wxALL, 10);

    panel->SetSizer(mainSizer);
    Centre();

    RefreshRentals();
}

wxString RentWindow::FormatTimePoint(const std::chrono::system_clock::time_point& tp) {
    std::time_t time = std::chrono::system_clock::to_time_t(tp);
    std::tm tm = *std::localtime(&time);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M");
    return wxString(oss.str());
}

void RentWindow::OnRentMovie(wxCommandEvent& event) {
    long movieID;
    if (!movieIdCtrl->GetValue().ToLong(&movieID)) {
        wxMessageBox("Please enter a valid Movie ID", "Error", wxOK | wxICON_ERROR);
        return;
    }

    User* user = systemManager.getCurrentUser();
    if (!user || user->customerID == -1) {
        wxMessageBox("No customer logged in", "Error", wxOK | wxICON_ERROR);
        return;
    }

    try {
        systemManager.rentMovie(user->customerID, movieID);
        wxMessageBox("Movie rented successfully!", "Success", wxOK | wxICON_INFORMATION);
        RefreshRentals();
    }
    catch (const std::exception& e) {
        wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR);
    }
}

void RentWindow::OnReturnMovie(wxCommandEvent& event) {
    long movieID;
    if (!movieIdCtrl->GetValue().ToLong(&movieID)) {
        wxMessageBox("Please enter a valid Movie ID", "Error", wxOK | wxICON_ERROR);
        return;
    }

    User* user = systemManager.getCurrentUser();
    if (!user || user->customerID == -1) {
        wxMessageBox("No customer logged in", "Error", wxOK | wxICON_ERROR);
        return;
    }

    try {
        systemManager.returnMovie(user->customerID, movieID);
        wxMessageBox("Movie returned successfully!", "Success", wxOK | wxICON_INFORMATION);
        RefreshRentals();
    }
    catch (const std::exception& e) {
        wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR);
    }
}

void RentWindow::OnSearchMovie(wxCommandEvent& event) {
    SearchWindow searchDlg(this, systemManager);
    if (searchDlg.ShowModal() == wxID_OK) {
        int selectedId = searchDlg.GetSelectedMovieId();
        movieIdCtrl->SetValue(wxString::Format("%d", selectedId));
    }
}

void RentWindow::OnUndoAction(wxCommandEvent& event) {
    try {
        systemManager.undoLastAction();
        wxMessageBox("Last action undone successfully!", "Success", wxOK | wxICON_INFORMATION);
        RefreshRentals();
    }
    catch (const std::exception& e) {
        wxMessageBox(e.what(), "Error", wxOK | wxICON_ERROR);
    }
}

void RentWindow::RefreshRentals() {
    rentalsGrid->Freeze();
    rentalsGrid->ClearGrid();
    if (rentalsGrid->GetNumberRows() > 0) {
        rentalsGrid->DeleteRows(0, rentalsGrid->GetNumberRows());
    }

    User* user = systemManager.getCurrentUser();
    if (!user || user->customerID == -1) {
        rentalsGrid->Thaw();
        return;
    }

    Customer* customer = systemManager.getCustomer(user->customerID);
    if (!customer) {
        rentalsGrid->Thaw();
        return;
    }

    // Get active rentals
    std::queue<int> tempQueue = customer->activeRentals;
    int row = 0;
    while (!tempQueue.empty()) {
        int movieID = tempQueue.front();
        tempQueue.pop();

        Movie* movie = systemManager.getMovie(movieID);
        if (!movie) continue;

        // Simplified rental record (in real system, would query actual records)
        RentalRecord record;
        record.customerID = customer->id;
        record.movieID = movieID;
        record.rentDate = std::chrono::system_clock::now() - std::chrono::hours(24);
        record.dueDate = record.rentDate + std::chrono::hours(72);

        rentalsGrid->AppendRows(1);
        rentalsGrid->SetCellValue(row, 0, wxString::Format("%d", movieID));
        rentalsGrid->SetCellValue(row, 1, movie->title);
        rentalsGrid->SetCellValue(row, 2, FormatTimePoint(record.rentDate));
        rentalsGrid->SetCellValue(row, 3, FormatTimePoint(record.dueDate));

        row++;
    }
    rentalsGrid->AutoSizeColumns();
    rentalsGrid->Thaw();
}
