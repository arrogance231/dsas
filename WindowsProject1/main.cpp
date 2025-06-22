#include <wx/wx.h>
#include "MainWindow.h"

class RentalApp : public wxApp {
public:
    virtual bool OnInit() override {
        MainWindow* mainWin = new MainWindow("Movie Rental System");
        mainWin->Show(true);
        return true;
    }
};

wxIMPLEMENT_APP(RentalApp);
