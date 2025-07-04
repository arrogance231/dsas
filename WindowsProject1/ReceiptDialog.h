

#ifndef RECEIPTDIALOG_H
#define RECEIPTDIALOG_H

#include <wx/wx.h>
#include "core.h"

class ReceiptDialog : public wxDialog {
public:
    ReceiptDialog(wxWindow* parent,
        const Movie& movie,
        const std::chrono::system_clock::time_point& rentDate,
        const std::chrono::system_clock::time_point& dueDate,
        const std::chrono::system_clock::time_point* returnDate = nullptr);

private:
    wxButton* closeButton;
    void OnClose(wxCommandEvent& event);
};

#endif // RECEIPTDIALOG_H
