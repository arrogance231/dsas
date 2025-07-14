// Made by Ram.
#pragma once
#ifndef RETURN_WINDOW_H
#define RETURN_WINDOW_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "../../core.h"

class ReturnWindow : public wxFrame {
public:
    ReturnWindow(SystemManager* systemManager);

private:
    SystemManager* systemManager;
    wxListCtrl* rentalList;
    wxButton* returnButton;
	wxButton* cancelButton;
    int selectedMovieID = -1;

    void PopulateRentalList();
    void OnMovieSelected(wxListEvent& event);
    void OnReturnClicked(wxCommandEvent& event);
    void OnMovieDeselected(wxListEvent& event);  
	void OnCancelClicked(wxCommandEvent& event);
};

#endif
