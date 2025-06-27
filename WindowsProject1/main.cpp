#include <wx/wx.h>
#include "include/gui/LogSignWindow.h"
#include "core.h"

class RentalApp : public wxApp {
public:
    virtual bool OnInit() override {
        // Create the system manager
        SystemManager* sysManager = new SystemManager();

        // Load movies from assets or saved file
        sysManager->loadMoviesFromFile("movies.json");
        if (sysManager->getAllMovies().empty()) {
            // If no saved movies, load from assets
            sysManager->loadMoviesFromAssets();
            // Save to file for future use
            sysManager->saveMoviesToFile("movies.json");
        }

        // Load users and admins from assets
        sysManager->loadUsersFromAssets();

        // Create and show the login window
        LogSignWindow* logSignWin = new LogSignWindow(nullptr, sysManager);
        logSignWin->Show(true);
        return true;
    }
    
    virtual int OnExit() override {
        // Save movies when app closes
        SystemManager* sysManager = nullptr;
        // You might want to store the system manager as a member variable
        // For now, we'll just return normally
        return wxApp::OnExit();
    }
};

wxIMPLEMENT_APP(RentalApp);
