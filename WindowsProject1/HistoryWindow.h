#pragma once
#ifndef HISTORYWINDOW_H
#define HISTORYWINDOW_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include "../../core.h"

// Made by Ram
class HistoryWindow : public wxFrame {
public:
    HistoryWindow(SystemManager* sysMgr, int custID);

private:
    void PopulateList();
    void OnClose(wxCommandEvent& event);

    SystemManager* systemManager;
    int customerID;
    wxListCtrl* rentalList;
    wxButton* closeButton;
};

#endif
