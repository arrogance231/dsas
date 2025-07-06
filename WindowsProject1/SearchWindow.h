#ifndef SEARCHWINDOW_H
#define SEARCHWINDOW_H

#include "../../core.h"
#include <wx/wx.h>
#include <wx/listctrl.h>

class SearchWindow : public wxDialog {
public:
    SearchWindow(wxWindow* parent, SystemManager& systemManager);
    int GetSelectedMovieId() const { return selectedMovieId; }

private:
    enum {
        ID_SearchText = wxID_HIGHEST + 100,
        ID_SearchBtn
    };

    void OnSearch(wxCommandEvent& event);
    void OnItemSelected(wxListEvent& event);
    void PopulateResults(const wxString& query, const wxString& genreFilter);

    SystemManager& systemManager;
    wxTextCtrl* searchText;
    wxListCtrl* resultsList;
    int selectedMovieId;

    wxDECLARE_EVENT_TABLE();
};

#endif
