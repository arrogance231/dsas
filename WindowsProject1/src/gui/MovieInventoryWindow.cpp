#include "../../include/gui/MovieInventoryWindow.h"
#include <wx/sizer.h>
#include "../../utils.h"

MovieInventoryWindow::MovieInventoryWindow(const wxString& title, SystemManager* sysManager)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)), systemManager(sysManager) {
    
    CreateControls();
    BindEventHandlers();
    PopulateMovieList();
}

void MovieInventoryWindow::CreateControls() {
    panel = new wxPanel(this);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
    // Movie List
    movieListCtrl = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL);
    movieListCtrl->AppendColumn("ID", wxLIST_FORMAT_LEFT, 50);
    movieListCtrl->AppendColumn("Title", wxLIST_FORMAT_LEFT, 200);
    movieListCtrl->AppendColumn("Genre", wxLIST_FORMAT_LEFT, 100);
    movieListCtrl->AppendColumn("Release Date", wxLIST_FORMAT_LEFT, 100);
    movieListCtrl->AppendColumn("Copies", wxLIST_FORMAT_LEFT, 80);
    movieListCtrl->AppendColumn("Description", wxLIST_FORMAT_LEFT, 300);
    mainSizer->Add(movieListCtrl, 1, wxEXPAND | wxALL, 10);

    // Input Form
    wxFlexGridSizer* formSizer = new wxFlexGridSizer(2, 10, 10);
    formSizer->AddGrowableCol(1);

    titleTextCtrl = new wxTextCtrl(panel, wxID_ANY, "");
    genreChoice = new wxChoice(panel, wxID_ANY);
    genreChoice->Append("Action");
    genreChoice->Append("Comedy");
    genreChoice->Append("Drama");
    copiesTextCtrl = new wxTextCtrl(panel, wxID_ANY, "");
    releaseDateTextCtrl = new wxTextCtrl(panel, wxID_ANY, "");
    descriptionTextCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    
    formSizer->Add(new wxStaticText(panel, wxID_ANY, "Title:"), 0, wxALIGN_CENTER_VERTICAL);
    formSizer->Add(titleTextCtrl, 1, wxEXPAND);
    formSizer->Add(new wxStaticText(panel, wxID_ANY, "Genre:"), 0, wxALIGN_CENTER_VERTICAL);
    formSizer->Add(genreChoice, 1, wxEXPAND);
    formSizer->Add(new wxStaticText(panel, wxID_ANY, "Release Date:"), 0, wxALIGN_CENTER_VERTICAL);
    formSizer->Add(releaseDateTextCtrl, 1, wxEXPAND);
    formSizer->Add(new wxStaticText(panel, wxID_ANY, "Copies:"), 0, wxALIGN_CENTER_VERTICAL);
    formSizer->Add(copiesTextCtrl, 1, wxEXPAND);
    formSizer->Add(new wxStaticText(panel, wxID_ANY, "Description:"), 0, wxALIGN_CENTER_VERTICAL);
    formSizer->Add(descriptionTextCtrl, 1, wxEXPAND);

    mainSizer->Add(formSizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);

    // Action Buttons
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    addButton = new wxButton(panel, wxID_ANY, "Add");
    editButton = new wxButton(panel, wxID_ANY, "Edit");
    deleteButton = new wxButton(panel, wxID_ANY, "Delete");
    
    editButton->Disable();
    deleteButton->Disable();

    buttonSizer->Add(addButton, 0, wxALL, 5);
    buttonSizer->Add(editButton, 0, wxALL, 5);
    buttonSizer->Add(deleteButton, 0, wxALL, 5);

    mainSizer->Add(buttonSizer, 0, wxALIGN_CENTER | wxBOTTOM, 10);

    panel->SetSizer(mainSizer);
}

void MovieInventoryWindow::BindEventHandlers() {
    addButton->Bind(wxEVT_BUTTON, &MovieInventoryWindow::OnAdd, this);
    editButton->Bind(wxEVT_BUTTON, &MovieInventoryWindow::OnEdit, this);
    deleteButton->Bind(wxEVT_BUTTON, &MovieInventoryWindow::OnDelete, this);
    movieListCtrl->Bind(wxEVT_LIST_ITEM_SELECTED, &MovieInventoryWindow::OnSelect, this);
}

void MovieInventoryWindow::PopulateMovieList() {
    movieListCtrl->DeleteAllItems();
    std::vector<Movie> movies = systemManager->getAllMovies();
    for (const auto& movie : movies) {
        long index = movieListCtrl->InsertItem(movieListCtrl->GetItemCount(), wxString::Format(wxT("%i"), movie.id));
        movieListCtrl->SetItem(index, 1, movie.title);
        movieListCtrl->SetItem(index, 2, wxString::FromUTF8(genreToString(movie.genre)));
        movieListCtrl->SetItem(index, 3, movie.releaseDate);
        movieListCtrl->SetItem(index, 4, wxString::Format(wxT("%i"), movie.copiesAvailable));
        movieListCtrl->SetItem(index, 5, movie.description);
        movieListCtrl->SetItemData(index, movie.id);
    }
}

void MovieInventoryWindow::OnAdd(wxCommandEvent& event) {
    wxString title = titleTextCtrl->GetValue();
    int genreIndex = genreChoice->GetSelection();
    wxString copiesStr = copiesTextCtrl->GetValue();

    if (title.IsEmpty() || copiesStr.IsEmpty() || genreIndex == wxNOT_FOUND) {
        wxMessageBox("Please fill all fields.", "Error", wxOK | wxICON_ERROR);
        return;
    }
    long copies;
    copiesStr.ToLong(&copies);

    Movie newMovie;
    newMovie.id = systemManager->getAllMovies().size() + 1; // Simple ID generation
    newMovie.title = title.ToStdString();
    newMovie.genre = static_cast<Genre>(genreIndex);
    newMovie.copiesAvailable = copies;
    
    systemManager->addMovie(newMovie);
    PopulateMovieList();

    titleTextCtrl->Clear();
    copiesTextCtrl->Clear();
    genreChoice->SetSelection(wxNOT_FOUND);
}

void MovieInventoryWindow::OnEdit(wxCommandEvent& event) {
    if (selectedMovieId == -1) return;

    Movie updatedMovie;
    updatedMovie.id = selectedMovieId;
    updatedMovie.title = titleTextCtrl->GetValue().ToStdString();
    updatedMovie.genre = static_cast<Genre>(genreChoice->GetSelection());
    long copies;
    copiesTextCtrl->GetValue().ToLong(&copies);
    updatedMovie.copiesAvailable = copies;

    systemManager->editMovie(selectedMovieId, updatedMovie);
    PopulateMovieList();
}

void MovieInventoryWindow::OnDelete(wxCommandEvent& event) {
    if (selectedMovieId == -1) return;

    systemManager->removeMovie(selectedMovieId);
    PopulateMovieList();

    titleTextCtrl->Clear();
    copiesTextCtrl->Clear();
    genreChoice->SetSelection(wxNOT_FOUND);
    editButton->Disable();
    deleteButton->Disable();
    selectedMovieId = -1;
}

void MovieInventoryWindow::OnSelect(wxListEvent& event) {
    selectedMovieId = event.GetItem().GetData();
    
    Movie* movie = systemManager->getMovie(selectedMovieId);
    if (movie) {
        titleTextCtrl->SetValue(movie->title);
        genreChoice->SetSelection(static_cast<int>(movie->genre));
        copiesTextCtrl->SetValue(wxString::Format(wxT("%i"), movie->copiesAvailable));
        editButton->Enable();
        deleteButton->Enable();
    }
} 