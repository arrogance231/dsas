// Made by Ram.
#ifndef CURRENT_RENTALS_WINDOW_H
#define CURRENT_RENTALS_WINDOW_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "../../core.h"  

class CurrentRentalsWindow : public wxFrame {
public:
    CurrentRentalsWindow(SystemManager* systemManager);

private:
    SystemManager* systemManager;
    wxListCtrl* rentalList;
    wxButton* cancelButton;

    void PopulateRentalList();
    void OnCancelClicked(wxCommandEvent& event);
};

#endif
