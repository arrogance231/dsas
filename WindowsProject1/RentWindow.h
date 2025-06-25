#ifndef RENTWINDOW_H
#define RENTWINDOW_H

#include "core.h"
#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/sizer.h>
#include <wx/panel.h>

class SearchWindow; // Forward declaration

class RentWindow : public wxFrame {
public:
    RentWindow(wxWindow* parent, SystemManager& systemManager);

private:
    enum {
        ID_RentBtn = wxID_HIGHEST + 1,
        ID_ReturnBtn,
        ID_SearchBtn,
        ID_UndoBtn
    };

    void OnRentMovie(wxCommandEvent& event);
    void OnReturnMovie(wxCommandEvent& event);
    void OnSearchMovie(wxCommandEvent& event);
    void OnUndoAction(wxCommandEvent& event);
    void RefreshRentals();
    wxString FormatTimePoint(const std::chrono::system_clock::time_point& tp);

    SystemManager& systemManager;
    wxGrid* rentalsGrid;
    wxTextCtrl* movieIdCtrl;
    wxButton* rentBtn;
    wxButton* returnBtn;
    wxButton* searchBtn;
    wxButton* undoBtn;

    wxDECLARE_EVENT_TABLE();
};

#endif // RENTWINDOW_H
