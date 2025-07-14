#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <wx/wx.h>
#include "../../core.h"

// Made by Arjhine A. Ty
class MainWindow : public wxFrame {
public:
    MainWindow(const wxString& title, SystemManager* sysManager);

private:
    // GUI Controls
    wxPanel* panel;
    wxButton* customerButton;
    wxButton* movieButton;
    wxButton* logoutButton;

    // System Data
    SystemManager* systemManager;

    // Event Handlers
    void OnManageCustomers(wxCommandEvent& event);
    void OnManageMovies(wxCommandEvent& event);
    void OnLogout(wxCommandEvent& event);
};

#endif
