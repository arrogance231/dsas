#ifndef CUSTOMERWINDOW_HPP
#define CUSTOMERWINDOW_HPP

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/msgdlg.h>
#include "../../core.h"

class CustomerWindow : public wxFrame {
public:
    CustomerWindow(const wxString& title, SystemManager* sysManager);

private:
    void CreateControls();
    void BindEventHandlers();
    void PopulateCustomerList();

    // Event Handlers
    void OnAdd(wxCommandEvent& event);
    void OnEdit(wxCommandEvent& event);
    void OnDelete(wxCommandEvent& event);
    void OnSelect(wxListEvent& event);

    // GUI Controls
    wxPanel* panel;
    wxListCtrl* customerListCtrl;
    wxTextCtrl* nameTextCtrl;
    wxTextCtrl* phoneTextCtrl;
    wxTextCtrl* emailTextCtrl;
    wxButton* addButton;
    wxButton* editButton;
    wxButton* deleteButton;

    // System Data
    SystemManager* systemManager;
    long selectedCustomerId = -1;
};

#endif // CUSTOMERWINDOW_HPP 