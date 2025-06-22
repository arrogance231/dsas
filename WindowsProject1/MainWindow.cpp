#include "MainWindow.h"

MainWindow::MainWindow(const wxString& title)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(600, 400)) {

    panel = new wxPanel(this, wxID_ANY);
    new wxStaticText(panel, wxID_ANY, "Welcome to the Movie Rental System!",
        wxPoint(20, 20), wxSize(300, 30));
}
