#include "MainPanel.h"

MainPanel::MainPanel(wxWindow *parent,const wxSize &size):wxPanel(parent,wxID_ANY,wxDefaultPosition,size)
{
    Bind(wxEVT_PAINT,&MainPanel::DrawMainPanel,this);
}

void MainPanel::DrawMainPanel(wxPaintEvent &event)
{
    wxPaintDC mainPanel(this);
    mainPanel.DrawBitmap(wxBitmap("Background.bmp",wxBITMAP_TYPE_BMP),0,0);
    mainPanel.SetFont(wxFont(std::min(GetSize().GetWidth()/20,GetSize().GetHeight()/12),wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,"Segoe UI"));
    mainPanel.DrawText("Echilibrul Chimic",GetSize().GetWidth()/2-mainPanel.GetTextExtent("Echilibrul Chimic").GetWidth()/2,3*GetSize().GetHeight()/10);
}
