#include "MainFrame.h"

MainFrame::MainFrame(const wxString &title):wxFrame(nullptr,wxID_ANY,title,wxDefaultPosition,wxDisplay().GetClientArea().GetSize())
{
    mainPanel=new MainPanel(this,GetSize());
    reactionPanel=nullptr;
    theoryPanel=nullptr;
    wxArrayString reactions;
    reactions.Add(wxT("FeCl\u2083 + 3NH\u2084SCN \u21CC Fe(SCN)\u2083 + 3NH\u2084Cl"));
    reactions.Add(wxT("CoCl\u2082 + 2KSCN \u21CC Co(SCN)\u2083 + 2KCl"));
    float scaleX=(float)GetSize().GetWidth()/1200;
    float scaleY=(float)GetSize().GetHeight()/720;
    choices=new wxChoice(mainPanel,wxID_ANY,wxDefaultPosition,wxSize(380*scaleX,-1),reactions);
    choices->SetSelection(0);
    choices->Center();
    choices->SetFont(wxFont(16*min(scaleX,scaleY),wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,"Segoe UI"));
    choices->Bind(wxEVT_CHOICE,&MainFrame::ReactionChoice,this);
    wxButton *theoryButton=new wxButton(mainPanel,wxID_ANY,"Teorie",wxPoint(-1,410*scaleY),wxSize(75*scaleX,30*scaleY));
    theoryButton->Center(wxHORIZONTAL);
    theoryButton->SetFont(wxFont(16*min(scaleX,scaleY),wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,"Segoe UI"));
    theoryButton->Bind(wxEVT_BUTTON,&MainFrame::PressedTheory,this);
    wxImage::AddHandler(new wxPNGHandler);
    wxBitmap exitBMP=wxBitmap("ExitButton.png",wxBITMAP_TYPE_PNG);
    wxButton *exitButton=new wxButton(mainPanel,wxID_ANY,wxEmptyString,wxPoint(GetSize().GetWidth()-exitBMP.GetWidth()+15,-1),wxSize(exitBMP.GetWidth()-15,exitBMP.GetHeight()-15));
    exitButton->SetBitmap(exitBMP);
    exitButton->Bind(wxEVT_BUTTON,&MainFrame::CloseApp,this);
    reactionColours[0][0]=wxColour(255,200,100);
    reactionColours[0][1]=wxColour(240,100,50);
    reactionColours[0][2]=wxColour(200,0,0);
    reactionColours[1][0]=wxColour(240,60,160);
    reactionColours[1][1]=wxColour(60,20,180);
    reactionColours[1][2]=wxColour(0,0,200);
}

void MainFrame::ReactionChoice(wxCommandEvent &event)
{
    mainPanel->Hide();
    if(reactionPanel==nullptr)
    {
        reactionPanel=new ReactionPanel(this,GetSize());
        reactionPanel->SetDoubleBuffered(true);
        wxBitmap returnBMP=wxBitmap("ReturnButton.png",wxBITMAP_TYPE_PNG);
        wxButton *returnButton1=new wxButton(reactionPanel,wxID_ANY,wxEmptyString,wxDefaultPosition,wxSize(returnBMP.GetWidth()-10,returnBMP.GetHeight()-10));
        returnButton1->SetBitmap(returnBMP);
        returnButton1->Bind(wxEVT_BUTTON,&MainFrame::ReturnToMainPanel,this);
        wxBitmap resetBMP=wxBitmap("ResetButton.png",wxBITMAP_TYPE_PNG);
        wxButton *resetButton=new wxButton(reactionPanel,wxID_ANY,wxEmptyString,wxPoint(GetSize().GetWidth()-resetBMP.GetWidth()+10,-1),wxSize(resetBMP.GetWidth()-10,resetBMP.GetHeight()-10));
        resetButton->SetBitmap(resetBMP);
        resetButton->Bind(wxEVT_BUTTON,&MainFrame::ResetReaction,this);
    }
    else
    {
        reactionPanel->Show();
    }
    reactionPanel->InitializeReaction(choices->GetString(choices->GetSelection()),reactionColours[choices->GetSelection()]);
}
void MainFrame::PressedTheory(wxCommandEvent &event)
{
    mainPanel->Hide();
    if(theoryPanel==nullptr)
    {
        theoryPanel=new TheoryPanel(this,GetSize());
        wxBitmap returnBMP=wxBitmap("ReturnButton.png",wxBITMAP_TYPE_PNG);
        wxButton *returnButton2=new wxButton(theoryPanel,wxID_ANY,wxEmptyString,wxDefaultPosition,wxSize(returnBMP.GetWidth()-10,returnBMP.GetHeight()-10));
        returnButton2->SetBitmap(returnBMP);
        returnButton2->Bind(wxEVT_BUTTON,&MainFrame::ReturnToMainPanel,this);
    }
    else
    {
        theoryPanel->Show();
    }
}
void MainFrame::ReturnToMainPanel(wxCommandEvent &event)
{
    if(reactionPanel!=nullptr)
    {
        reactionPanel->Hide();
    }
    if(theoryPanel!=nullptr)
    {
        theoryPanel->Hide();
    }
    mainPanel->Show();
}
void MainFrame::ResetReaction(wxCommandEvent &event)
{
    reactionPanel->InitializeReaction(choices->GetString(choices->GetSelection()),reactionColours[choices->GetSelection()]);
    reactionPanel->Refresh();
}
void MainFrame::CloseApp(wxCommandEvent &event)
{
    Close();
}
