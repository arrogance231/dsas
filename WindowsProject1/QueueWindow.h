#ifndef QUEUEWINDOW_H
#define QUEUEWINDOW_H

// wxWidgets UI components
#include <wx/wx.h>
#include <wx/listbox.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <vector>

// Include core logic (movies, users, waitlist management)
#include "core.h"

// QueueWindow class definition
class QueueWindow : public wxDialog {
public:
    // Constructor to open window without a preselected movie
    QueueWindow(wxWindow* parent, SystemManager& systemManager);

    // Constructor to open window with a specific movie preselected
    QueueWindow(wxWindow* parent, SystemManager& systemManager, int preselectedMovieID);

private:
    // Reference to the system manager handling logic
    SystemManager& systemManager;

    // UI components
    wxChoice* movieChoice;      // Dropdown for selecting a movie
    wxListBox* waitlistBox;     // Shows the list of customers in waitlist
    wxButton* joinButton;       // Button to join the waitlist
    wxButton* leaveButton;      // Button to leave the waitlist
    wxButton* closeButton;      // Button to close the window

    // Event handlers
    void OnMovieSelection(wxCommandEvent& event);  // When a movie is selected
    void OnJoinWaitlist(wxCommandEvent& event);    // When "Join" is clicked
    void OnLeaveWaitlist(wxCommandEvent& event);   // When "Leave" is clicked
    void OnClose(wxCommandEvent& event);           // When "Close" is clicked

    // Refresh the waitlist display based on the selected movie
    void RefreshWaitlistDisplay();

    // wxWidgets macro to declare the event table
    wxDECLARE_EVENT_TABLE();
};

#endif
