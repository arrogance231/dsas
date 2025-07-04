#ifndef USERMENUWINDOW_HPP
#define USERMENUWINDOW_HPP

#include <wx/wx.h>
#include "../../core.h"

class UserMenuWindow : public wxFrame {
public:
    UserMenuWindow(SystemManager* sysManager);
private:
    SystemManager* systemManager;
    wxButton* rentMovieBtn;
    wxButton* returnMovieBtn;
    wxButton* checkRentalsBtn;
    wxButton* rentalHistoryBtn;
    wxButton* checkInvoiceBtn;
    void OnRentMovie(wxCommandEvent& event);
    void OnReturnMovie(wxCommandEvent& event);
	void OnRentalHistory(wxCommandEvent& event); // Added for rental history functionality
    void OnCheckRentals(wxCommandEvent& event);
    void OnCheckInvoice(wxCommandEvent& event);
};

#endif // USERMENUWINDOW_HPP 