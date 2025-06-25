#include "SearchWindow.h"
#include <wx/msgdlg.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/listctrl.h>
#include <wx/colour.h>

wxBEGIN_EVENT_TABLE(SearchWindow, wxDialog)
EVT_TEXT_ENTER(ID_SearchText, SearchWindow::OnSearch)
EVT_BUTTON(ID_SearchBtn, SearchWindow::OnSearch)
EVT_LIST_ITEM_ACTIVATED(wxID_ANY, SearchWindow::OnItemSelected)
wxEND_EVENT_TABLE()

SearchWindow::SearchWindow(wxWindow* parent, SystemManager& sysMgr)
    : wxDialog(parent, wxID_ANY, "Search Movies", wxDefaultPosition, wxSize(600, 400)),
    systemManager(sysMgr), selectedMovieId(-1) {

    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);

    // Search box
    wxBoxSizer* searchSizer = new wxBoxSizer(wxHORIZONTAL);
    searchText = new wxTextCtrl(this, ID_SearchText, "", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
    searchSizer->Add(searchText, 1, wxEXPAND | wxRIGHT, 5);

    wxButton* searchBtn = new wxButton(this, ID_SearchBtn, "Search");
    searchSizer->Add(searchBtn, 0);
    mainSizer->Add(searchSizer, 0, wxEXPAND | wxALL, 10);

    // Results list
    resultsList = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxLC_REPORT | wxLC_SINGLE_SEL);
    resultsList->AppendColumn("ID", wxLIST_FORMAT_LEFT, 50);
    resultsList->AppendColumn("Title", wxLIST_FORMAT_LEFT, 200);
    resultsList->AppendColumn("Genre", wxLIST_FORMAT_LEFT, 100);
    resultsList->AppendColumn("Available", wxLIST_FORMAT_LEFT, 80);
    mainSizer->Add(resultsList, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

    // Buttons
    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
    btnSizer->Add(new wxButton(this, wxID_CANCEL, "Cancel"), 0, wxRIGHT, 10);
    btnSizer->Add(new wxButton(this, wxID_OK, "Select"), 0);
    mainSizer->Add(btnSizer, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 10);

    SetSizer(mainSizer);
    searchText->SetFocus();
}

void SearchWindow::OnSearch(wxCommandEvent& event) {
    PopulateResults(searchText->GetValue());
}

void SearchWindow::OnItemSelected(wxListEvent& event) {
    selectedMovieId = wxAtoi(resultsList->GetItemText(event.GetIndex()));
    EndModal(wxID_OK);
}

void SearchWindow::PopulateResults(const wxString& query) {
    resultsList->DeleteAllItems();
    if (query.empty()) return;

    wxString searchTerm = query.Lower();
    auto allMovies = systemManager.getAllMovies();

    for (const auto& movie : allMovies) {
        if (wxString(movie.title).Lower().Contains(searchTerm)) {
            long index = resultsList->InsertItem(resultsList->GetItemCount(),
                wxString::Format("%d", movie.id));
            resultsList->SetItem(index, 1, movie.title);
            resultsList->SetItem(index, 2, movie.genre);
            resultsList->SetItem(index, 3, movie.copiesAvailable > 0 ? "Yes" : "No");

            if (movie.copiesAvailable <= 0) {
                for (int col = 0; col < 4; col++) {
                    resultsList->SetItemTextColour(index, wxColour(200, 200, 200));
                }
            }
        }
    }
}
