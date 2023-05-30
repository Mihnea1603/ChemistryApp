#include <wx/app.h>
#include "MainFrame.h"

class App:public wxApp
{
public:
    bool OnInit()
    {
        MainFrame *mainFrame=new MainFrame("ChemistryApp");
        mainFrame->Center();
        mainFrame->ShowFullScreen(true);
        return true;
    }
};

wxIMPLEMENT_APP(App);
