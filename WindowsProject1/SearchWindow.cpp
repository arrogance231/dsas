#include "SearchWindow.h"
#include <wx/msgdlg.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/listctrl.h>
#include <wx/colour.h>

// Made by Ezekiel
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

    // Genre filter dropdown
    wxArrayString genres;
    genres.Add("All");
    genres.Add("Action");
    genres.Add("Comedy");
    genres.Add("Drama");
    genres.Add("Horror");
    genres.Add("Anime");
    genres.Add("Romance");
    wxChoice* genreChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, genres);
    genreChoice->SetSelection(0);
    searchSizer->Add(genreChoice, 0, wxLEFT, 5);

    mainSizer->Add(searchSizer, 0, wxEXPAND | wxALL, 10);

    // Results list
    resultsList = new wxListCtrl(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxLC_REPORT | wxLC_SINGLE_SEL);
    resultsList->AppendColumn("ID", wxLIST_FORMAT_LEFT, 50);
    resultsList->AppendColumn("Title", wxLIST_FORMAT_LEFT, 150);
    resultsList->AppendColumn("Genre", wxLIST_FORMAT_LEFT, 80);
    resultsList->AppendColumn("Release Date", wxLIST_FORMAT_LEFT, 80);
    resultsList->AppendColumn("Available", wxLIST_FORMAT_LEFT, 70);
    resultsList->AppendColumn("Description", wxLIST_FORMAT_LEFT, 200);
    mainSizer->Add(resultsList, 1, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

    // Buttons
    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
    btnSizer->Add(new wxButton(this, wxID_CANCEL, "Cancel"), 0, wxRIGHT, 10);
    wxButton* selectBtn = new wxButton(this, wxID_OK, "Select");
    btnSizer->Add(selectBtn, 0);
    mainSizer->Add(btnSizer, 0, wxALIGN_RIGHT | wxRIGHT | wxBOTTOM, 10);

    SetSizer(mainSizer);
    searchText->SetFocus();

    // Show all movies by default
    PopulateResults("", "All");

    // Remove Filter button and bind search box to filter as you type
    searchText->Bind(wxEVT_TEXT, [this, genreChoice](wxCommandEvent&) {
        PopulateResults(searchText->GetValue(), genreChoice->GetStringSelection());
    });
    genreChoice->Bind(wxEVT_CHOICE, [this, genreChoice](wxCommandEvent&) {
        PopulateResults(searchText->GetValue(), genreChoice->GetStringSelection());
    });

    // Bind select button
    selectBtn->Bind(wxEVT_BUTTON, [this](wxCommandEvent&) {
        long selected = resultsList->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
        if (selected != -1) {
            selectedMovieId = wxAtoi(resultsList->GetItemText(selected));
            EndModal(wxID_OK);
        }
    });
}

void SearchWindow::OnSearch(wxCommandEvent& event) {
    // Do nothing, search is now only on Filter button
}

void SearchWindow::OnItemSelected(wxListEvent& event) {
    selectedMovieId = wxAtoi(resultsList->GetItemText(event.GetIndex()));
    EndModal(wxID_OK);
}

void SearchWindow::PopulateResults(const wxString& query, const wxString& genreFilter) {
    resultsList->DeleteAllItems();
    wxString searchTerm = query.Lower();
    auto allMovies = systemManager.getAllMovies();
    for (const auto& movie : allMovies) {
        wxString movieGenre = wxString::FromUTF8(genreToString(movie.genre));
        bool genreMatch = (genreFilter == "All" || movieGenre == genreFilter);
        bool searchMatch = searchTerm.IsEmpty() || wxString(movie.title).Lower().Contains(searchTerm) || wxString(movie.description).Lower().Contains(searchTerm);
        if (genreMatch && searchMatch) {
            long index = resultsList->InsertItem(resultsList->GetItemCount(), wxString::Format("%d", movie.id));
            resultsList->SetItem(index, 1, movie.title);
            resultsList->SetItem(index, 2, movieGenre);
            resultsList->SetItem(index, 3, movie.releaseDate);
            resultsList->SetItem(index, 4, movie.copiesAvailable > 0 ? "Yes" : "No");
            resultsList->SetItem(index, 5, movie.description);
            if (movie.copiesAvailable <= 0) {
                for (int col = 0; col < 6; col++) {
                    resultsList->SetItemTextColour(index, wxColour(200, 200, 200));
                }
            }
        }
    }
}
