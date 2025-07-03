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
    wxButton* checkQueueBtn; //Queue BTN
	wxButton* checkOverdueBtn; // Overdue Window BTN
	wxButton* checkFeeBtn; // Fee Window BTN
    wxButton* checkRentalsBtn;
    wxButton* checkInvoiceBtn;
    void OnRentMovie(wxCommandEvent& event);
    void OnReturnMovie(wxCommandEvent& event);
	void OnCheckQueue(wxCommandEvent& event); //Queue BTN
	void OnCheckOverdue(wxCommandEvent& event); // Overdue Window BTN
	void OnCheckFee(wxCommandEvent& event); // Fee Window BTN
    void OnCheckRentals(wxCommandEvent& event);
    void OnCheckInvoice(wxCommandEvent& event);
};

#endif // USERMENUWINDOW_HPP 