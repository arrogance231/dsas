// Made by Sam.
#ifndef OVERDUEWINDOW_H
#define OVERDUEWINDOW_H

//  wxWidgets headers for GUI components
#include <wx/wx.h>
#include <wx/grid.h>

//  Include the core logic (movies, customers, rentals)
#include "../../core.h"

//  OverdueWindow Class Declaration
class OverdueWindow : public wxDialog {
public:

    //  Constructor to show overdue rentals for a SPECIFIC movie
    OverdueWindow(wxWindow* parent, SystemManager& systemManager, int movieID);
    // Constructor to show overdue rentals for a SPECIFIC user
    OverdueWindow(wxWindow* parent, SystemManager& systemManager, int userID, bool forUser);

private:
    SystemManager& systemManager;  // Reference to the system manager (handles data)
    wxGrid* grid;                  // Grid to display overdue info

    //  Helper functions to populate the grid
    void PopulateGrid();                // For all overdue rentals
    void PopulateGridForMovie(int movieID);  // For a specific movie
    void PopulateGridForUser(int userID);    // For a specific user
};

#endif // End of header guard
