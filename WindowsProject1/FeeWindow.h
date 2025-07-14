// Made by Sam.
#ifndef FEEWINDOW_H
#define FEEWINDOW_H

#include <wx/wx.h>
#include "../../core.h"

class FeeWindow : public wxDialog {
public:
    FeeWindow(wxWindow* parent, SystemManager& systemManager, int movieID);

private:
    void calculateAndShowFee(SystemManager& systemManager, int movieID);
};

#endif
