#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <wx/wx.h>

class MainWindow : public wxFrame {
public:
    MainWindow(const wxString& title);

private:
    wxPanel* panel;
};

#endif
