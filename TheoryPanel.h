#include <wx/panel.h>
#include <wx/dcclient.h>
#include <wx/tokenzr.h>

class TheoryPanel:public wxPanel
{
public:
    TheoryPanel(wxWindow *parent,const wxSize &size);
private:
    float scaleX,scaleY;
    void DrawTheoryPanel(wxPaintEvent &event);
    void DrawParagraph(wxDC &theoryPanel,wxString &paragraph,int x,int &y);
};
