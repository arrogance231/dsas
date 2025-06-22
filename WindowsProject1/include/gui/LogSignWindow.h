#ifndef LOGSIGNWINDOW_HPP
#define LOGSIGNWINDOW_HPP

#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/choice.h>
#include <wx/msgdlg.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include "../../core.h"

class LogSignWindow : public wxFrame {
public:
    LogSignWindow(const wxString& title, SystemManager* sysManager);

private:
    // GUI elements
    wxNotebook* notebook;
    wxPanel* loginPanel;
    wxPanel* signupPanel;
    
    // Login panel elements
    wxTextCtrl* loginUsername;
    wxTextCtrl* loginPassword;
    wxButton* loginButton;
    
    // Signup panel elements
    wxTextCtrl* signupUsername;
    wxTextCtrl* signupPassword;
    wxTextCtrl* signupConfirmPassword;
    wxTextCtrl* signupName;
    wxTextCtrl* signupPhone;
    wxTextCtrl* signupEmail;
    wxChoice* signupRole;
    wxButton* signupButton;
    
    // System manager reference
    SystemManager* systemManager;
    
    // Event handlers
    void OnLogin(wxCommandEvent& event);
    void OnSignup(wxCommandEvent& event);
    void OnClose(wxCloseEvent& event);
    
    // Helper functions
    void CreateLoginPanel();
    void CreateSignupPanel();
    bool ValidateSignupInputs();
    void ShowError(const wxString& message);
    void ShowSuccess(const wxString& message);
};

#endif 