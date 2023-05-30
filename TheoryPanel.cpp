#include "TheoryPanel.h"

TheoryPanel::TheoryPanel(wxWindow *parent,const wxSize &size):wxPanel(parent,wxID_ANY,wxDefaultPosition,size)
{
    scaleX=(float)GetSize().GetWidth()/1200;
    scaleY=(float)GetSize().GetHeight()/720;
    Bind(wxEVT_PAINT,&TheoryPanel::DrawTheoryPanel,this);
}

void TheoryPanel::DrawTheoryPanel(wxPaintEvent &event)
{
    wxPaintDC theoryPanel(this);
    theoryPanel.DrawBitmap(wxBitmap("Whiteboard.bmp",wxBITMAP_TYPE_BMP),0,0);
    theoryPanel.SetFont(wxFont(48*std::min(scaleX,scaleY),wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,"Segoe UI"));
    theoryPanel.SetTextForeground(wxColour(255,255,255));
    theoryPanel.DrawText("Principiul Le Chatelier",GetSize().GetWidth()/2-theoryPanel.GetTextExtent("Principiul Le Chatelier").GetWidth()/2,36*scaleY);
    theoryPanel.SetFont(wxFont(20*std::min(scaleX,scaleY),wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,"Segoe UI"));
    wxString paragraph1="Daca asupra unui sistem aflat in echilibru chimic se exercita o actiune din exterior, "
                        "echilibrul se deplaseaza in sensul diminuarii sau anularii actiunii exercitate.";
    wxString paragraph2="Factorii care influenteaza deplasarea echilibrului chimic sunt:\n"
                        "- variatia concentratiei reactantilor sau a produsilor de reactie din amestecul aflat in echilibru: "
                        "cresterea concentratiei reactantilor deplaseaza echilibrul chimic in directia formarii produsilor "
                        "de reactie, in timp ce cresterea concentratiei produsilor de reactie deplaseaza echilibrul chimic "
                        "in directia produsilor initiali;\n"
                        "- variatia temperaturii in functie de caracterul exoterm sau endoterm al celor doua reactii "
                        "(reactia directa si reactia inversa): cresterea temperaturii va deplasa echilibrul chimic in sensul "
                        "favorizarii reactiei endoterme(-Q), iar micsorarea temperaturii va deplasa echilibrul chimic in "
                        "sensul favorizarii reactiei exoterme(+Q);\n"
                        "- variatia presiunii, in cazul echilibrelor care implica substante gazoase si in care are loc variatia "
                        "numarului de moli de gaz: cresterea presiunii va deplasa echilibrul chimic in directia in care se "
                        "afla mai putini moli de gaz, iar micsorarea presiunii deplaseaza echilibrul chimic in directia in "
                        "care se afla mai multi moli de gaz.";
    int y=150*scaleY;
    DrawParagraph(theoryPanel,paragraph1,125*scaleX,y);
    DrawParagraph(theoryPanel,paragraph2,125*scaleX,y);
}
void TheoryPanel::DrawParagraph(wxDC &theoryPanel,wxString &paragraph,int x,int &y)
{
    wxArrayString lines;
    wxStringTokenizer tokenizer(paragraph,"\n");
    while(tokenizer.HasMoreTokens()==1)
    {
        wxStringTokenizer wordTokenizer(tokenizer.GetNextToken()," ");
        while (wordTokenizer.HasMoreTokens()==1)
        {
            lines.Add(wordTokenizer.GetNextToken());
        }
        lines.Add("");
    }
    for(wxString &line:lines)
    {
        if(line=="")
        {
            y+=theoryPanel.GetCharHeight();
            x=75*scaleX;
        }
        else
        {
            if(x+theoryPanel.GetTextExtent(line).GetWidth()>GetSize().GetWidth()-75*scaleX)
            {
                y+=theoryPanel.GetCharHeight();
                x=75*scaleX;
            }
            theoryPanel.DrawText(line,x,y);
            x+=theoryPanel.GetTextExtent(line).GetWidth()+theoryPanel.GetTextExtent(" ").GetWidth();
        }
    }
}
