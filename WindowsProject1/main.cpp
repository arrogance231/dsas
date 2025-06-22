#include <wx/wx.h>
#include "include/gui/LogSignWindow.h"
#include "core.h"

class RentalApp : public wxApp {
public:
    virtual bool OnInit() override {
        // Create the system manager
        SystemManager* sysManager = new SystemManager();

        // Create and show the login window
        LogSignWindow* logSignWin = new LogSignWindow("Login or Sign Up", sysManager);
        logSignWin->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(RentalApp);
