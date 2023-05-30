#include <wx/panel.h>
#include <wx/dcclient.h>

class MainPanel:public wxPanel
{
public:
    MainPanel(wxWindow *parent,const wxSize &size);
private:
    void DrawMainPanel(wxPaintEvent &event);
};
