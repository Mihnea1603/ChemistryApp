#include "ReactionPanel.h"

ReactionPanel::ReactionPanel(wxWindow *parent,const wxSize &size):wxPanel(parent,wxID_ANY,wxDefaultPosition,size)
{
    background=wxBitmap("Background.bmp",wxBITMAP_TYPE_BMP);
    scaleX=(float)GetSize().GetWidth()/1200;
    scaleY=(float)GetSize().GetHeight()/720;
    hintBox=wxRect(GetSize().GetWidth()-200*scaleX,50*scaleY,180*scaleX,90*scaleX);
    for(int i=0; i<4; i++)
    {
        tubes[i].timer=nullptr;
        tubes[i].rect=wxRect((30+130*i)*scaleX,70*scaleY,100*scaleX,600*scaleY);
        if(i<3)
        {
            jars[i].rect=wxRect((660+180*i)*scaleX,150*scaleY,150*scaleX,500*scaleY);
        }
    }
    jars[0].equilibriumChange=jars[1].equilibriumChange=1;
    jars[2].equilibriumChange=-1;
    jars[0].brush=wxBRUSHSTYLE_SOLID;
    jars[1].brush=jars[2].brush=wxBRUSHSTYLE_TRANSPARENT;
    Bind(wxEVT_PAINT,&ReactionPanel::DrawReactionPanel,this);
    Bind(wxEVT_LEFT_DOWN,&ReactionPanel::MouseClick,this);
    Bind(wxEVT_LEFT_DCLICK,&ReactionPanel::MouseDoubleClick,this);
    Bind(wxEVT_MOTION,&ReactionPanel::MouseMove,this);
    Bind(wxEVT_LEFT_UP,&ReactionPanel::MouseRelease,this);
}
void ReactionPanel::InitializeReaction(wxString reaction,wxColour reactionColours[])
{
    reactionEquation=reaction;
    pipette.brush=wxBRUSHSTYLE_TRANSPARENT;
    clickedPipette=0;
    pipetteFilled=0;
    aboveTube=-1;
    aboveJar=-1;
    for(int i=0; i<4; i++)
    {
        tubes[i].equilibrium=1;
        tubes[i].colour=reactionColours[1];
        tubes[i].extra=tubes[i].inReaction=tubes[i].factor=0;
        if(i<3)
        {
            jars[i].colour=reactionColours[i];
            targetColours[i]=wxColour(reactionColours[i].Red()/2+reactionColours[1].Red()/2,reactionColours[i].Green()/2+reactionColours[1].Green()/2,reactionColours[i].Blue()/2+reactionColours[1].Blue()/2);
        }
    }
    wxStringTokenizer tokenizer(reaction," ");
    int i=0,j=0;
    while(tokenizer.HasMoreTokens()==1)
    {
        substances[i]=tokenizer.GetNextToken();
        if(i%2==0 && i!=4)
        {
            jars[j].substance=substances[i];
            if(j!=0)
            {
                jars[j].substance.Remove(0,1);
            }
            j++;
        }
        i++;
    }
}

void ReactionPanel::DrawReactionPanel(wxPaintEvent &event)
{
    wxPaintDC reactionPanel(this);
    reactionPanel.DrawBitmap(background,0,0);
    reactionPanel.SetFont(wxFont(32*min(scaleX,scaleY),wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,"Segoe UI"));
    for(int i=0; i<7; i++)
    {
        substancePosition=0;
        for(int j=0; j<i; j++)
        {
            substancePosition+=reactionPanel.GetTextExtent(substances[j]).GetWidth()+reactionPanel.GetTextExtent(" ").GetWidth();
        }
        if(i==0 || i==4)
        {
            reactionPanel.SetTextForeground(jars[i/2].colour);
        }
        else
        {
            reactionPanel.SetTextForeground(wxColour(0,0,0));
        }
        reactionPanel.DrawText(substances[i],GetSize().GetWidth()/2-reactionPanel.GetTextExtent(reactionEquation).GetWidth()/2+substancePosition,25*scaleY);
    }
    for(int i=0; i<4; i++)
    {
        reactionPanel.SetPen(wxPen(wxColour(100,200,255),4*min(scaleX,scaleY),wxPENSTYLE_TRANSPARENT));
        reactionPanel.SetBrush(wxBrush(tubes[i].colour,wxBRUSHSTYLE_SOLID));
        reactionPanel.DrawRectangle(wxPoint(tubes[i].rect.x,tubes[i].rect.y+310*scaleY),wxSize(100*scaleX,250*scaleY));
        reactionPanel.SetBrush(wxBrush(wxColour(tubes[i].colour.Red()*0.8,tubes[i].colour.Green()*0.8,tubes[i].colour.Blue()*0.8),wxBRUSHSTYLE_SOLID));
        reactionPanel.DrawEllipse(wxPoint(tubes[i].rect.x,tubes[i].rect.y+300*scaleY),wxSize(100*scaleX,20*scaleY));
        reactionPanel.SetPen(wxPen(wxColour(100,200,255),4*min(scaleX,scaleY),wxPENSTYLE_SOLID));
        reactionPanel.SetBrush(wxBrush(tubes[i].colour,wxBRUSHSTYLE_SOLID));
        reactionPanel.DrawEllipticArc(tubes[i].rect.x,tubes[i].rect.y+510*scaleY,100*scaleX,90*scaleY,180,0);
        reactionPanel.SetBrush(wxBrush(tubes[i].colour,wxBRUSHSTYLE_TRANSPARENT));
        reactionPanel.DrawEllipse(wxPoint(tubes[i].rect.x,tubes[i].rect.y+200*scaleY),wxSize(100*scaleX,20*scaleY));
        reactionPanel.DrawLine(wxPoint(tubes[i].rect.x,tubes[i].rect.y+210*scaleY),wxPoint(tubes[i].rect.x,tubes[i].rect.y+560*scaleY));
        reactionPanel.DrawLine(wxPoint(tubes[i].rect.x+100*scaleX,tubes[i].rect.y+210*scaleY),wxPoint(tubes[i].rect.x+100*scaleX,tubes[i].rect.y+560*scaleY));
        if(tubes[i].extra==1)
        {
            reactionPanel.SetPen(wxPen(wxColour(0,0,0),4*min(scaleX,scaleY),wxPENSTYLE_SOLID));
            reactionPanel.SetFont(wxFont(18*min(scaleX,scaleY),wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,"Segoe UI"));
            reactionPanel.DrawText("Extra",tubes[i].rect.x+50*scaleX-reactionPanel.GetTextExtent("Extra").GetWidth()/2,tubes[i].rect.y+360*scaleY);
            reactionPanel.DrawText(tubes[i].substances[0],tubes[i].rect.x+50*scaleX-reactionPanel.GetTextExtent(tubes[i].substances[0]).GetWidth()/2,tubes[i].rect.y+385*scaleY);
            reactionPanel.DrawText(tubes[i].plusSign,tubes[i].rect.x+50*scaleX-reactionPanel.GetTextExtent(tubes[i].plusSign).GetWidth()/2,tubes[i].rect.y+410*scaleY);
            reactionPanel.DrawText(tubes[i].substances[1],tubes[i].rect.x+50*scaleX-reactionPanel.GetTextExtent(tubes[i].substances[1]).GetWidth()/2,tubes[i].rect.y+435*scaleY);
            reactionPanel.DrawLine(wxPoint((tubes[i].rect.x+20*scaleX)*tubes[i].factor+(tubes[i].rect.x+35*scaleX)*(1-tubes[i].factor),tubes[i].rect.y+620*scaleY),wxPoint((tubes[i].rect.x+80*scaleX)*tubes[i].factor+(tubes[i].rect.x+60*scaleX)*(1-tubes[i].factor),tubes[i].rect.y+620*scaleY));
            if(tubes[i].equilibrium!=0)
            {
                reactionPanel.DrawLine(wxPoint((tubes[i].rect.x+80*scaleX)*tubes[i].factor+(tubes[i].rect.x+65*scaleX)*(1-tubes[i].factor),tubes[i].rect.y+620*scaleY),wxPoint((tubes[i].rect.x+80*scaleX)*tubes[i].factor+(tubes[i].rect.x+65*scaleX)*(1-tubes[i].factor)-10,tubes[i].rect.y+610*scaleY));
                reactionPanel.DrawLine(wxPoint((tubes[i].rect.x+80*scaleX)*tubes[i].factor+(tubes[i].rect.x+65*scaleX)*(1-tubes[i].factor),tubes[i].rect.y+620*scaleY),wxPoint((tubes[i].rect.x+80*scaleX)*tubes[i].factor+(tubes[i].rect.x+65*scaleX)*(1-tubes[i].factor)-10,tubes[i].rect.y+630*scaleY));
            }
            if(tubes[i].equilibrium!=2)
            {
                reactionPanel.DrawLine(wxPoint((tubes[i].rect.x+20*scaleX)*tubes[i].factor+(tubes[i].rect.x+35*scaleX)*(1-tubes[i].factor),tubes[i].rect.y+620*scaleY),wxPoint((tubes[i].rect.x+20*scaleX)*tubes[i].factor+(tubes[i].rect.x+35*scaleX)*(1-tubes[i].factor)+10,tubes[i].rect.y+610*scaleY));
                reactionPanel.DrawLine(wxPoint((tubes[i].rect.x+20*scaleX)*tubes[i].factor+(tubes[i].rect.x+35*scaleX)*(1-tubes[i].factor),tubes[i].rect.y+620*scaleY),wxPoint((tubes[i].rect.x+20*scaleX)*tubes[i].factor+(tubes[i].rect.x+35*scaleX)*(1-tubes[i].factor)+10,tubes[i].rect.y+630*scaleY));
            }

        }
    }
    for(int i=0; i<3; i++)
    {
        reactionPanel.SetPen(wxPen(wxColour(100,200,255),4*min(scaleX,scaleY),wxPENSTYLE_TRANSPARENT));
        reactionPanel.SetBrush(wxBrush(jars[i].colour,jars[i].brush));
        reactionPanel.DrawRectangle(wxPoint(jars[i].rect.x,jars[i].rect.y+350*scaleY),wxSize(150*scaleX,140*scaleY));
        reactionPanel.DrawRectangle(wxPoint(jars[i].rect.x+10*scaleX,jars[i].rect.y+490*scaleY),wxSize(130*scaleX,10*scaleY));
        reactionPanel.SetBrush(wxBrush(wxColour(jars[i].colour.Red()*0.8,jars[i].colour.Green()*0.8,jars[i].colour.Blue()*0.8),jars[i].brush));
        reactionPanel.DrawEllipse(wxPoint(jars[i].rect.x,jars[i].rect.y+340*scaleY),wxSize(150*scaleX,20*scaleY));
        reactionPanel.SetPen(wxPen(wxColour(100,200,255),4*min(scaleX,scaleY),wxPENSTYLE_SOLID));
        reactionPanel.SetBrush(wxBrush(jars[i].colour,jars[i].brush));
        reactionPanel.DrawEllipticArc(jars[i].rect.x,jars[i].rect.y+480*scaleY,20*scaleX,20*scaleY,180,270);
        reactionPanel.DrawEllipticArc(jars[i].rect.x+130*scaleX,jars[i].rect.y+480*scaleY,20*scaleX,20*scaleY,270,0);
        reactionPanel.SetBrush(wxBrush(jars[i].colour,wxBRUSHSTYLE_TRANSPARENT));
        reactionPanel.DrawEllipse(wxPoint(jars[i].rect.x+45*scaleX,jars[i].rect.y+200*scaleY),wxSize(60*scaleX,10*scaleY));
        reactionPanel.DrawLine(wxPoint(jars[i].rect.x+45*scaleX,jars[i].rect.y+205*scaleY),wxPoint(jars[i].rect.x+45*scaleX,jars[i].rect.y+250*scaleY));
        reactionPanel.DrawLine(wxPoint(jars[i].rect.x+105*scaleX,jars[i].rect.y+205*scaleY),wxPoint(jars[i].rect.x+105*scaleX,jars[i].rect.y+250*scaleY));
        reactionPanel.DrawLine(wxPoint(jars[i].rect.x+45*scaleX,jars[i].rect.y+250*scaleY),wxPoint(jars[i].rect.x,jars[i].rect.y+300*scaleY));
        reactionPanel.DrawLine(wxPoint(jars[i].rect.x+105*scaleX,jars[i].rect.y+250*scaleY),wxPoint(jars[i].rect.x+150*scaleX,jars[i].rect.y+300*scaleY));
        reactionPanel.DrawLine(wxPoint(jars[i].rect.x,jars[i].rect.y+300*scaleY),wxPoint(jars[i].rect.x,jars[i].rect.y+490*scaleY));
        reactionPanel.DrawLine(wxPoint(jars[i].rect.x+150*scaleX,jars[i].rect.y+300*scaleY),wxPoint(jars[i].rect.x+150*scaleX,jars[i].rect.y+490*scaleY));
        reactionPanel.DrawLine(wxPoint(jars[i].rect.x+10*scaleX,jars[i].rect.y+500*scaleY),wxPoint(jars[i].rect.x+140*scaleX,jars[i].rect.y+500*scaleY));
        reactionPanel.SetFont(wxFont(22*min(scaleX,scaleY),wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,"Segoe UI"));
        reactionPanel.DrawText(jars[i].substance,jars[i].rect.x+75*scaleX-reactionPanel.GetTextExtent(jars[i].substance).GetWidth()/2,jars[i].rect.y+380*scaleY);
    }
    reactionPanel.DrawRoundedRectangle(hintBox,5*min(scaleX,scaleY));
    reactionPanel.SetFont(wxFont(12*min(scaleX,scaleY),wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,"Segoe UI"));
    if(pipetteFilled==1)
    {
        if(aboveTube!=-1)
        {
            reactionPanel.DrawText("Dati dublu-click",hintBox.x+90*scaleX-reactionPanel.GetTextExtent("Dati dublu-click").GetWidth()/2,hintBox.y+20*scaleY);
            reactionPanel.DrawText("pe pipeta",hintBox.x+90*scaleX-reactionPanel.GetTextExtent("pe pipeta").GetWidth()/2,hintBox.y+40*scaleY);
        }
        else
        {
            reactionPanel.DrawText("Mutati pipeta",hintBox.x+90*scaleX-reactionPanel.GetTextExtent("Mutati pipeta").GetWidth()/2,hintBox.y+20*scaleY);
            reactionPanel.DrawText("deasupra unei eprubete",hintBox.x+90*scaleX-reactionPanel.GetTextExtent("deasupra unei eprubete").GetWidth()/2,hintBox.y+40*scaleY);
        }
    }
    else
    {
        if(aboveJar!=-1)
        {
            reactionPanel.DrawText("Dati dublu-click",hintBox.x+90*scaleX-reactionPanel.GetTextExtent("Dati dublu-click").GetWidth()/2,hintBox.y+20*scaleY);
            reactionPanel.DrawText("pe pipeta",hintBox.x+90*scaleX-reactionPanel.GetTextExtent("pe pipeta").GetWidth()/2,hintBox.y+40*scaleY);
        }
        else if(aboveTube!=-1 && tubes[aboveTube].inReaction==1)
        {
            reactionPanel.DrawText("Felicitari!",hintBox.x+90*scaleX-reactionPanel.GetTextExtent("Felicitari!").GetWidth()/2,hintBox.y+10*scaleY);
            reactionPanel.DrawText("Puteti observa",hintBox.x+90*scaleX-reactionPanel.GetTextExtent("Puteti observa").GetWidth()/2,hintBox.y+30*scaleY);
            reactionPanel.DrawText("deplasarea echilibrului",hintBox.x+90*scaleX-reactionPanel.GetTextExtent("deplasarea echilibrului").GetWidth()/2,hintBox.y+50*scaleY);
        }
        else
        {
            reactionPanel.DrawText("Mutati pipeta",hintBox.x+90*scaleX-reactionPanel.GetTextExtent("Mutati pipeta").GetWidth()/2,hintBox.y+20*scaleY);
            reactionPanel.DrawText("deasupra unui borcan",hintBox.x+90*scaleX-reactionPanel.GetTextExtent("deasupra unui borcan").GetWidth()/2,hintBox.y+40*scaleY);
        }
    }
    if(clickedPipette==0)
    {
        if(aboveTube!=-1)
        {
            pipette.rect=wxRect(tubes[aboveTube].rect.GetPosition().x+30*scaleX,tubes[aboveTube].rect.GetPosition().y+10*scaleY,40*scaleX,180*scaleY);
        }
        else if(aboveJar!=-1)
        {
            pipette.rect=wxRect(jars[aboveJar].rect.GetPosition().x+55*scaleX,jars[aboveJar].rect.GetPosition().y+10*scaleY,40*scaleX,180*scaleY);
        }
        else
        {
            pipette.rect=wxRect(jars[0].rect.x-70*scaleX,jars[0].rect.y+260*scaleY,40*scaleX,180*scaleY);
        }
    }
    reactionPanel.SetPen(wxPen(wxColour(100,200,255),4*min(scaleX,scaleY),wxPENSTYLE_TRANSPARENT));
    reactionPanel.SetBrush(wxBrush(pipette.colour,pipette.brush));
    reactionPanel.DrawRectangle(wxPoint(pipette.rect.x,pipette.rect.y+40*scaleY),wxSize(40*scaleX,25*scaleY));
    reactionPanel.DrawRectangle(wxPoint(pipette.rect.x+10*scaleX,pipette.rect.y+65*scaleY),wxSize(20*scaleX,90*scaleY));
    pipetteTip[0]=wxPoint(pipette.rect.x+10*scaleX,pipette.rect.y+155*scaleY);
    pipetteTip[1]=wxPoint(pipette.rect.x+30*scaleX,pipette.rect.y+155*scaleY);
    pipetteTip[2]=wxPoint(pipette.rect.x+24*scaleX,pipette.rect.y+180*scaleY);
    pipetteTip[3]=wxPoint(pipette.rect.x+16*scaleX,pipette.rect.y+180*scaleY);
    reactionPanel.DrawPolygon(4,pipetteTip);
    reactionPanel.SetBrush(wxBrush(wxColour(pipette.colour.Red()*0.8,pipette.colour.Green()*0.8,pipette.colour.Blue()*0.8),pipette.brush));
    reactionPanel.DrawEllipse(wxPoint(pipette.rect.x,pipette.rect.y+35*scaleY),wxSize(40*scaleX,10*scaleY));
    reactionPanel.SetPen(wxPen(wxColour(100,200,255),4*min(scaleX,scaleY),wxPENSTYLE_SOLID));
    reactionPanel.SetBrush(wxBrush(pipette.colour,pipette.brush));
    reactionPanel.DrawEllipticArc(pipette.rect.x,pipette.rect.y+50*scaleY,40*scaleX,30*scaleY,180,240);
    reactionPanel.DrawEllipticArc(pipette.rect.x,pipette.rect.y+50*scaleY,40*scaleX,30*scaleY,300,0);
    reactionPanel.SetBrush(wxBrush(pipette.colour,wxBRUSHSTYLE_TRANSPARENT));
    reactionPanel.DrawEllipticArc(pipette.rect.x,pipette.rect.y,40*scaleX,30*scaleY,0,180);
    reactionPanel.DrawLine(wxPoint(pipette.rect.x,pipette.rect.y+15*scaleY),wxPoint(pipette.rect.x,pipette.rect.y+65*scaleY));
    reactionPanel.DrawLine(wxPoint(pipette.rect.x+40*scaleX,pipette.rect.y+15*scaleY),wxPoint(pipette.rect.x+40*scaleX,pipette.rect.y+65*scaleY));
    reactionPanel.DrawLine(wxPoint(pipette.rect.x+10*scaleX,pipette.rect.y+80*scaleY),wxPoint(pipette.rect.x+10*scaleX,pipette.rect.y+155*scaleY));
    reactionPanel.DrawLine(wxPoint(pipette.rect.x+30*scaleX,pipette.rect.y+80*scaleY),wxPoint(pipette.rect.x+30*scaleX,pipette.rect.y+155*scaleY));
    reactionPanel.DrawLine(wxPoint(pipette.rect.x+10*scaleX,pipette.rect.y+155*scaleY),wxPoint(pipette.rect.x+16*scaleX,pipette.rect.y+180*scaleY));
    reactionPanel.DrawLine(wxPoint(pipette.rect.x+30*scaleX,pipette.rect.y+155*scaleY),wxPoint(pipette.rect.x+24*scaleX,pipette.rect.y+180*scaleY));
    if(pipetteFilled==1)
    {
        reactionPanel.SetFont(wxFont(10*min(scaleX,scaleY),wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_BOLD,false,"Segoe UI"));
        reactionPanel.DrawText(pipette.substance,pipette.rect.x+20*scaleX-reactionPanel.GetTextExtent(pipette.substance).GetWidth()/2,pipette.rect.y+50*scaleY);
    }

}
void ReactionPanel::MouseClick(wxMouseEvent &event)
{
    if (pipette.rect.Contains(event.GetPosition())==1)
    {
        clickedPipette=1;
        mouseOffset=event.GetPosition()-pipette.rect.GetPosition();
    }
}
void ReactionPanel::MouseDoubleClick(wxMouseEvent &event)
{
    if (pipette.rect.Contains(event.GetPosition())==1)
    {
        if(aboveTube!=-1 && pipetteFilled==1 && tubes[aboveTube].equilibrium+equilibriumChange>=0 && tubes[aboveTube].equilibrium+equilibriumChange<=2)
        {
            pipetteFilled=0;
            pipette.brush=wxBRUSHSTYLE_TRANSPARENT;
            tubes[aboveTube].equilibrium+=equilibriumChange;
            if(tubes[aboveTube].equilibrium!=1)
            {
                tubes[aboveTube].substances[0]=pipette.substance;
                tubes[aboveTube].substances[1]="";
                tubes[aboveTube].plusSign="";

            }
            else if(tubes[aboveTube].extra!=0)
            {
                if(pipette.substance!=jars[2].substance)
                {
                    tubes[aboveTube].substances[1]=tubes[aboveTube].substances[0];
                    tubes[aboveTube].substances[0]=pipette.substance;
                }
                else
                {
                    tubes[aboveTube].substances[1]=pipette.substance;
                }
                tubes[aboveTube].plusSign="+";
            }
            tubes[aboveTube].extra=1;
            tubes[aboveTube].inReaction=1;
            tubes[aboveTube].timerCounter=0;
            if(tubes[aboveTube].timer==nullptr)
            {
                tubes[aboveTube].timer=new wxTimer(this);
                Bind(wxEVT_TIMER,&ReactionPanel::ColourTimer,this);
            }
            tubes[aboveTube].timer->Start(10);
        }
        if(aboveJar!=-1)
        {
            pipetteFilled=1;
            pipette.colour=jars[aboveJar].colour;
            pipette.brush=jars[aboveJar].brush;
            pipette.substance=jars[aboveJar].substance;
            equilibriumChange=jars[aboveJar].equilibriumChange;
            Refresh();
        }
    }
}
void ReactionPanel::MouseMove(wxMouseEvent &event)
{
    if(clickedPipette!=0)
    {
        pipette.rect.SetPosition(event.GetPosition()-mouseOffset);
        Refresh();
    }
}
void ReactionPanel::MouseRelease(wxMouseEvent &event)
{
    if(clickedPipette==1)
    {
        for(int i=0; i<4; i++)
        {
            if(tubes[i].rect.Contains(event.GetPosition())==1)
            {
                aboveTube=i;
                break;
            }
            aboveTube=-1;
        }
        for(int i=0; i<3; i++)
        {
            if(jars[i].rect.Contains(event.GetPosition())==1)
            {
                aboveJar=i;
                break;
            }
            aboveJar=-1;
        }
        clickedPipette=0;
        Refresh();
    }
}
void ReactionPanel::ColourTimer(wxTimerEvent &event)
{
    for(int i=0; i<4; i++)
    {
        if(tubes[i].inReaction==1)
        {
            tubes[i].timerCounter++;
            tubes[i].factor=(float)tubes[i].timerCounter/100;
            tubes[i].colour=wxColour(targetColours[tubes[i].equilibrium].Red()*tubes[i].factor+tubes[i].colour.Red()*(1-tubes[i].factor),targetColours[tubes[i].equilibrium].Green()*tubes[i].factor+tubes[i].colour.Green()*(1-tubes[i].factor),targetColours[tubes[i].equilibrium].Blue()*tubes[i].factor+tubes[i].colour.Blue()*(1-tubes[i].factor));
            if(tubes[i].timerCounter==100)
            {
                tubes[i].inReaction=0;
                tubes[i].timer->Stop();
            }
        }
    }
    Refresh();
}

float min(float x,float y)
{
    if(x<y)
    {
        return x;
    }
    return y;
}
