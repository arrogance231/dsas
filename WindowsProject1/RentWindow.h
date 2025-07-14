// Made by Ezekiel.
#ifndef RENTWINDOW_H
#define RENTWINDOW_H

#include "../../core.h"
#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/sizer.h>
#include <wx/panel.h>

class SearchWindow; // Forward declaration

class RentWindow : public wxDialog {
public:
    RentWindow(wxWindow* parent, SystemManager& systemManager, int movieID);

private:
    enum {
        ID_RentBtn = wxID_HIGHEST + 1,
        ID_SearchBtn
    };

    void OnRentMovie(wxCommandEvent& event);
    void OnSearchMovie(wxCommandEvent& event);
    void RefreshRentals();
    wxString FormatTimePoint(const std::chrono::system_clock::time_point& tp);

    SystemManager& systemManager;
    int selectedMovieID;
    wxStaticText* movieTitleText;
    wxStaticText* movieGenreText;
    wxStaticText* movieReleaseDateText;
    wxStaticText* movieDescriptionText;
    wxButton* rentBtn;
    wxButton* searchBtn;

    wxDECLARE_EVENT_TABLE();
};

#endif // RENTWINDOW_H
