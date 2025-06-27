#ifndef MOVIEINVENTORYWINDOW_HPP
#define MOVIEINVENTORYWINDOW_HPP

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/msgdlg.h>
#include <wx/choice.h>
#include "../../core.h"

class MovieInventoryWindow : public wxFrame {
public:
    MovieInventoryWindow(const wxString& title, SystemManager* sysManager);

private:
    void CreateControls();
    void BindEventHandlers();
    void PopulateMovieList();

    // Event Handlers
    void OnAdd(wxCommandEvent& event);
    void OnEdit(wxCommandEvent& event);
    void OnDelete(wxCommandEvent& event);
    void OnSelect(wxListEvent& event);

    // GUI Controls
    wxPanel* panel;
    wxListCtrl* movieListCtrl;
    wxTextCtrl* titleTextCtrl;
    wxChoice* genreChoice;
    wxTextCtrl* copiesTextCtrl;
    wxTextCtrl* releaseDateTextCtrl;
    wxTextCtrl* descriptionTextCtrl;
    wxButton* addButton;
    wxButton* editButton;
    wxButton* deleteButton;

    // System Data
    SystemManager* systemManager;
    long selectedMovieId = -1;
};

#endif // MOVIEINVENTORYWINDOW_HPP 